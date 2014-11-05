/*
 * main.c
 *
 * Created: 24.8.2014 15:10:04
 *  Author: Tomas Baca
 */ 

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "system.h"
#include "mainTask.h"
#include "cspTask.h"
//#include "spi_memory_25lcxx.h"

// Blinking RTOS task, just for debugging
void blink(void *p) {
	
	while (1) {
				
		power_switch_toggle();		
        vTaskDelay(1000);
	}
}

// void testing (void *p)
// {
// complex cislo;
// complex cislo_read[5];
// char msg[64];
// 
// 
// 
// 	cislo.imag = 50;
// 	cislo.real = 180;	
// 
// 	spi_mem_write_complex(0x15,cislo);
// 	cislo_read[0] = spi_mem_read_complex(0x15);
// 	
// 	cislo.imag = 55;
// 	cislo.real = 190;
// 
// 	spi_mem_write_complex(0x754,cislo);
// 	cislo_read[1] = spi_mem_read_complex(0x754);
// 	
// 	cislo.imag = 60;
// 	cislo.real = 150;
// 
// 	spi_mem_write_complex(0x214,cislo);
// 	cislo_read[2] = spi_mem_read_complex(0x214);
// 	
// 	cislo.imag = 65;
// 	cislo.real = 610;
// 
// 	spi_mem_write_complex(0xd87,cislo);
// 	cislo_read[3] = spi_mem_read_complex(0xD87);
// 	
// 	cislo.imag = 70;
// 	cislo.real = 180;
// 
// 	spi_mem_write_complex(0x367,cislo);
// 	cislo_read[4] = spi_mem_read_complex(0x367);
// 	
// 	sprintf(msg, "\n\r*** Board test SPI memory***\n\rvalue: %i+j%i, %i+j%i, %i+j%i %i+j%i, %i+j%i\n\r",cislo_read[0].real,cislo_read[0].imag, 
// 																										cislo_read[1].real,cislo_read[1].imag,
// 																										cislo_read[2].real,cislo_read[2].imag,
// 																										cislo_read[3].real,cislo_read[3].imag,
// 																										cislo_read[4].real,cislo_read[4].imag);
// 
// // 	strcpy(outcomingPacket->data, msg);
// // 	outcomingPacket->length = strlen(msg);
// 
// 	/* Send packet */
// // 	if (csp_sendto(CSP_PRIO_NORM, CSP_OBC_ADDRESS, 15, 15, CSP_O_NONE, outcomingPacket, 1000) == CSP_ERR_NONE) {
// // 			
// // 		/* Send succeeded */
// // 	}
// // 	else {
// // 		/* Send failed */
// // 	}
// 		
// 		
// 	vTaskDelay(1000);
// }

int main(void) {
	
	// initialize the xMega peripherals
	boardInit();
	
	// Initialize the CSP buffers
	csp_buffer_init(CSP_BUFFER_COUNT, CSP_BUFFER_SIZE);
			
	// Initialize the CSP
	csp_init(CSP_MY_ADDRESS);
	
	// Initialize the CSP I2C interface
	csp_i2c_init(CSP_I2C_SLAVE_ADDRESS, 0, CSP_I2C_BAUDSETTING);
					
	// Add route to OBC via i2c
	csp_route_set(CSP_OBC_ADDRESS, &csp_if_i2c, CSP_I2C_OBC_ADDRESS);
					
	// Start router task
	csp_route_start_task(CSP_ROUTER_STACK, CSP_ROUTER_PRIORITY);
				
	/* -------------------------------------------------------------------- */
	/*	Starts blinking task - only for debug								*/
	/* -------------------------------------------------------------------- */
	xTaskCreate(blink, (signed char*) "blink", 64, NULL, configNORMAL_PRIORITY, NULL);
	
	//xTaskCreate(testing, (signed char*) "testing", 256, NULL, configNORMAL_PRIORITY, NULL);	
	/* -------------------------------------------------------------------- */
	/*	Starts task that handles incoming communication		 				*/
	/* -------------------------------------------------------------------- */
	xTaskCreate(cspTask, (signed char*) "cspTask", 512, NULL, configNORMAL_PRIORITY, NULL);
	
	/* -------------------------------------------------------------------- */
	/*	Starts task that handles outgoing communication		 				*/
	/* -------------------------------------------------------------------- */
	xTaskCreate(mainTask, (signed char*) "mainTask", 512, NULL, configNORMAL_PRIORITY, NULL);
	
	/* -------------------------------------------------------------------- */
	/*	Starts the scheduler and all previously created tasks				*/
	/* -------------------------------------------------------------------- */
	vTaskStartScheduler();
	
	return 0;
}