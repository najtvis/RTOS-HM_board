/*
 * system.c
 *
 * Created: 11.10.2014 18:40:47
 *  Author: Tomas Baca
 */

 #include "sysclk.h"
 #include "system.h"
 #include "TC_driver.h"
 #include "clksys_driver.h"
 #include "timers.h"
 //#include "spi_memory_25lcxx.h"
 
 volatile uint32_t milisecondsTimer;
 volatile uint32_t secondsTimer;
 volatile uint32_t hoursTimer;
 
 //An array to hold handles to the created timers.
 xTimerHandle xTimers[ NUM_TIMERS ];
 

/* -------------------------------------------------------------------- */
/*	Initialize the xMega peripherals									*/
/* -------------------------------------------------------------------- */
void boardInit() {
	
	// prepare i/o
	ioport_init();

	// clock init & enable system clock to all peripheral modules
	sysclk_init();
	
	// enable external oscillator
	//enable_xtal();
	
	// enable SPI communication on port C for SPI memory
	//spi_mem_init();	
	
	sysclk_enable_module(SYSCLK_PORT_GEN, 0xff);
	sysclk_enable_module(SYSCLK_PORT_A, 0xff);
	sysclk_enable_module(SYSCLK_PORT_B, 0xff);
	sysclk_enable_module(SYSCLK_PORT_C, 0xff);
	sysclk_enable_module(SYSCLK_PORT_D, 0xff);
	sysclk_enable_module(SYSCLK_PORT_E, 0xff);
	sysclk_enable_module(SYSCLK_PORT_F, 0xff);
		
/* -------------------------------------------------------------------- */
/*	Timer for RTC - SW timer											*/
/* -------------------------------------------------------------------- */
	
	xTimers[0] = xTimerCreate("timerRTC", 1000, pdTRUE,( void * ) 0, timerRTC);
	
	xTimerStart(xTimers[0], 0);
	
	milisecondsTimer = 0;
	secondsTimer = 0;
	hoursTimer = 0;
		
/* -------------------------------------------------------------------- */
/*	Timer for sampling													*/
/* -------------------------------------------------------------------- */
		
	// select the clock source and pre-scaler by 8
	//TC1_ConfigClockSource(&TCC1, TC_CLKSEL_DIV1_gc);
	
	//TC1_SetOverflowIntLevel(&TCC1, TC_OVFINTLVL_LO_gc);
	
	//TC_SetPeriod(&TCC1, F_CPU/SAMPLING_RATE-1);		// sampling rate 2kHz	
	
	
/* -------------------------------------------------------------------- */
/*	Setup I/Os															*/
/* -------------------------------------------------------------------- */
	
	ioport_set_pin_dir(POWER_SWITCH, IOPORT_DIR_OUTPUT);
	ioport_set_pin_high(POWER_SWITCH);
	
	ioport_set_pin_dir(MEM_CS,IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(MEM_MOSI,IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(MEM_MISO,IOPORT_DIR_INPUT);
	ioport_set_pin_dir(MEM_SCK,IOPORT_DIR_OUTPUT);
	
	//power_switch_off();
	
}

/* -------------------------------------------------------------------- */
/*	Interrupt for sampling input										*/
/* -------------------------------------------------------------------- */

ISR(TCC1_OVF_vect) {
	
	power_switch_toggle();
	//asm volatile ( "reti" );

}

/* -------------------------------------------------------------------- */
/*	Timing of the RTC every 1s											*/
/* -------------------------------------------------------------------- */
void timerRTC(void *p)
{
	if (secondsTimer++ == 3600) 
	{
		secondsTimer = 0;
		hoursTimer++;
	}
}


/* -------------------------------------------------------------------- */
/*	Enable external timing												*/
/* -------------------------------------------------------------------- */
void enable_xtal(void)
{
	PMIC.CTRL = PMIC.CTRL |	PMIC_MEDLVLEN_bm;
	// Enable external oscillator 16,470 MHz
	CLKSYS_XOSC_Config(OSC_FRQRANGE_12TO16_gc, 0, OSC_XOSCSEL_XTAL_1KCLK_gc);
	CLKSYS_Enable( OSC_XOSCEN_bm );
	while ( CLKSYS_IsReady( OSC_XOSCRDY_bm ) == 0 ){}
	CLKSYS_Main_ClockSource_Select( CLK_SCLKSEL_XOSC_gc );
	CLKSYS_Disable( OSC_RC2MEN_bm );
	// END - Enable external oscillator
}