/*
 * mainTask.c
 *
 * Created: 11.10.2014 20:18:00
 *  Author: Tomas Baca
 */


#include "mainTask.h"
#include "cspTask.h"
#include "system.h"
#include "spi_memory_23lcxx.h"
#include "config.h"
#include "fft_memory.h"
#include "TC_driver.h"
#include "twi_master_driver.h"
#include <math.h>
#include <string.h>

csp_packet_t * outcomingPacket;
extern unsigned int count_TCD0;

/* -------------------------------------------------------------------- */
/*	Send to the PC/OBC raw sampled signal								*/
/* -------------------------------------------------------------------- */
int processAll(csp_packet_t * inPacket) {
	char msg[20];
	
	count_TCD0 = 0;
	sampling = true;
	
	vTaskDelay(2000);
	while(sampling==true);
	
	power_switch_toggle();
	
	decimate_and_store();
	FFT_calculate();
	
	power_switch_toggle();
	
	sprintf(msg,"Sampling and computing done\n");
	strcpy(outcomingPacket->data, msg);
	outcomingPacket->length = strlen(msg);
	
	/* Send packet */
	csp_sendto(CSP_PRIO_NORM, inPacket->id.src, inPacket->id.sport, inPacket->id.dport, CSP_O_NONE, outcomingPacket, 1000);

	vTaskDelay(10);
	
	
	return 0;
}

/* -------------------------------------------------------------------- */
/*	Send to the PC/OBC raw sampled signal								*/
/* -------------------------------------------------------------------- */
int sendSignal(csp_packet_t * inPacket) {
	unsigned int i;
	int point;

	char msg[30];
	memset(msg,0,30);
		
	power_switch_toggle();
	
	TC0_ConfigClockSource(&TCC0, TC_CLKSEL_OFF_gc);	// STOP RTOS
	_delay_ms(4);
	
	sprintf(msg,"Sampled signal\n");
	strcpy(outcomingPacket->data, msg);
	outcomingPacket->length = strlen(msg);
			
	/* Send packet */
	csp_sendto(CSP_PRIO_NORM, inPacket->id.src, inPacket->id.sport, inPacket->id.dport, CSP_O_NONE, outcomingPacket, 100);

	_delay_ms(4);
			
	for (i=0;i<MEM_SIGNAL_LENGTH;i++)
	{
		point = spi_mem_read_word(i*2 + MEM_SIGNAL_BEGIN);
		
		sprintf(msg,"%i;%d\n",i*5,point);
		strcpy(outcomingPacket->data, msg);
		outcomingPacket->length = strlen(msg);
		
		/* Send packet */
		if (csp_sendto(CSP_PRIO_NORM, inPacket->id.src, inPacket->id.sport, inPacket->id.dport, CSP_O_NONE, outcomingPacket, 100) == CSP_ERR_NONE) {
			
			/* Send succeeded */
		} else 
		{
			/* Send failed */
		}
		_delay_ms(2);
		//vTaskDelay(7);
	}
	
	TC0_ConfigClockSource(&TCC0, TC_CLKSEL_DIV64_gc);	// Continue RTOS
	
	return 0;
}

/* -------------------------------------------------------------------- */
/*	Send to the PC/OBC raw sampled signal								*/
/* -------------------------------------------------------------------- */
int sendFFT(csp_packet_t * inPacket) {
	unsigned int i;
	float data2;
	complex point;
	int data;
	long f_axis;

	char msg[30];
	memset(msg,0,30);
	
	TC0_ConfigClockSource(&TCC0, TC_CLKSEL_OFF_gc);	// STOP RTOS
	_delay_ms(5);
	
	power_switch_toggle();
	
	sprintf(msg,"Sampled signal\n");
	strcpy(outcomingPacket->data, msg);
	outcomingPacket->length = strlen(msg);
	
	
	/* Send packet */
		csp_sendto(CSP_PRIO_NORM, inPacket->id.src, inPacket->id.sport, inPacket->id.dport, CSP_O_NONE, outcomingPacket, 15);

	
	for (i=0;i<NO_POINTS/2;i++)
	{
		point = spi_mem_read_complex(i*8 + MEM_FFT_BEGIN);
		point.real *= point.real;
		point.imag *=  point.imag;
		data2 =  log(point.real+point.imag)*10;
		data = (int) data2;
		
		f_axis = (((long) i)*10000)/NO_POINTS;
		
		memset(msg,0,30);
		
		sprintf(msg,"%i;%d\n",(int) f_axis, data);
		strcpy(outcomingPacket->data, msg);
		outcomingPacket->length = strlen(msg);
		
		/* Send packet */
		if (csp_sendto(CSP_PRIO_NORM, inPacket->id.src, inPacket->id.sport, inPacket->id.dport, CSP_O_NONE, outcomingPacket, 10) == CSP_ERR_NONE) {
			
			power_switch_toggle();
			
			/* Send succeeded */
		} else
		{
			/* Send failed */
		}
		_delay_ms(4);
	}

	TC0_ConfigClockSource(&TCC0, TC_CLKSEL_DIV64_gc);	// continue RTOS
	
	return 0;
}

/* -------------------------------------------------------------------- */
/*	Reply the free heap space in human readable form					*/
/* -------------------------------------------------------------------- */
int sendFreeHeapSpace(csp_packet_t * inPacket) {
	char msg[20];
	int temp;
	memset(msg,0,20);
	
	TC0_ConfigClockSource(&TCC0, TC_CLKSEL_OFF_gc);	// STOP RTOS
	
	temp = ADT_get_temperature();
	
	sprintf(msg,"%i.%i\n",temp/128,(int)((((long)temp)*100)/128)%100);
	strcpy(outcomingPacket->data, msg);
	outcomingPacket->length = strlen(msg);
	csp_sendto(CSP_PRIO_NORM, inPacket->id.src, inPacket->id.sport, inPacket->id.dport, CSP_O_NONE, outcomingPacket, 10);
	
	TC0_ConfigClockSource(&TCC0, TC_CLKSEL_DIV64_gc);	// continue RTOS
	
// 	itoa(xPortGetFreeHeapSize(), msg, 10);
// 	
// 	/* Copy message to packet */
// 	
//  	strcpy(outcomingPacket->data, msg);
//  	outcomingPacket->length = strlen(msg);
// 		
// 	/* Send packet */
// 	if (csp_sendto(CSP_PRIO_NORM, inPacket->id.src, inPacket->id.sport, inPacket->id.dport, CSP_O_NONE, outcomingPacket, 1000) == CSP_ERR_NONE) {
// 			
// 		/* Send succeeded */
// 	} 
// 	else 
// 	{
// 		/* Send failed */
// 
// 	}
		

	return 0;
}

/* -------------------------------------------------------------------- */
/*	Reply with some status info message									*/
/* -------------------------------------------------------------------- */
int houseKeeping(csp_packet_t * inPacket) 
{
complex cislo;	
complex cislo_read;
uint16_t aa=7442,bb;
int cc;

cislo.imag = 1876.45;
cislo.real = -3.47;
	// put the info message into the packet
	char msg[64];
	memset(msg,0,64);
	
	TC0_ConfigClockSource(&TCC0, TC_CLKSEL_OFF_gc);	// STOP RTOS
	
	spi_mem_write_complex(145,cislo);
	cislo_read = spi_mem_read_complex(145);
	
	
 	spi_mem_write_word(0x789,aa);
 	bb = spi_mem_read_word(0x789);
	
	if (cislo_read.real == cislo.real)
	{
		bb += 1;
	}
	
	if (cislo_read.imag == cislo.imag)
	{
		bb += 2;
	}

	cc = adca_read_ch0();
	

	
	sprintf(msg, "*** Board\n\rSoftware v1.0\n\rUptime: %id %ih %im %ds\n\r", (int16_t) hoursTimer/24, (int16_t) hoursTimer%24, (int16_t) secondsTimer/60, (int16_t) secondsTimer%60);
	//sprintf(msg, "*** \n\r%1.3f, %1.3f \n\r%1.3f, %1.3f, %i, ADC=%i\n\r", cislo.real, cislo.imag, cislo_read.real, cislo_read.imag, bb, cc);

	strcpy(outcomingPacket->data, msg);
	outcomingPacket->length = strlen(msg);

	/* Send packet */
	if (csp_sendto(CSP_PRIO_NORM, inPacket->id.src, inPacket->id.sport, inPacket->id.dport, CSP_O_NONE, outcomingPacket, 50) == CSP_ERR_NONE) {
		
		/* Send succeeded */
		power_switch_toggle();
	} else {
		/* Send failed */
	}
	_delay_ms(300);

	TC0_ConfigClockSource(&TCC0, TC_CLKSEL_DIV64_gc);	// continue RTOS
	
	return 0;
}

/* -------------------------------------------------------------------- */
/*	Sends back the incoming packet										*/
/* -------------------------------------------------------------------- */
int echoBack(csp_packet_t * inPacket) {

	/* Send packet */
	// reuses the incoming packet for the response
	if (csp_sendto(CSP_PRIO_NORM, inPacket->id.src, inPacket->id.sport, inPacket->id.dport, CSP_O_NONE, inPacket, 1000) == CSP_ERR_NONE) {
		
		/* Send succeeded */
		power_switch_toggle();
	} else {
		/* Send failed */
	}

	return 0;
}

/* -------------------------------------------------------------------- */
/*	The main task														*/
/* -------------------------------------------------------------------- */
void mainTask(void *p) {
	
	/* The variable used to receive from the queue. */
	xCSPStackEvent_t xReceivedEvent;
	
	outcomingPacket = csp_buffer_get(CSP_PACKET_SIZE);
	
	// infinite while loop of the program 
	while (1) {
		
		// the queue between cspTask and the main task
		// this is unblocking way how to read from the queue, the last parameter is "ticks to wait"
		if (xQueueReceive(xCSPEventQueue, &xReceivedEvent, 1)) {
		
			switch( xReceivedEvent.eEventType )
			{
				// Reply with RTOS free heap space
				// replies in Human Readable form
				case freeHeapEvent :
			
					sendFreeHeapSpace(xReceivedEvent.pvData);
			
				break;
			
				// Echo back the whole packet
				// incoming port => outcoming
				case echoBackEvent :
			
					echoBack(xReceivedEvent.pvData);
			
				break;
			
				// sends the info about the system
				case housKeepingEvent :
			
					houseKeeping(xReceivedEvent.pvData);
			
				break;
				
				// excite beam, measure his signal by piezo, compute FFT and attenuation
				case processAllEvent:
					processAll(xReceivedEvent.pvData);
				break;
				
				// send measured signal
				case sendSignalEvent:
					sendSignal(xReceivedEvent.pvData);
				break;
				
				// send computed FFT
				case sendFFTEvent:
					sendFFT(xReceivedEvent.pvData);
				break;
				
				// send computed attenuation signal
				case sendAttenuationEvent:
				
				break;
				
				// send summary info (2x resonant frequency, attenuation)
				case sendSummaryInfoEvent:
				
				break;
		
				default :
					/* Should not get here. */
				break;
			}
		}
	}
}