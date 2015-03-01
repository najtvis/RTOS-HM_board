/*
 * system.h
 *
 * Created: 11.10.2014 18:40:01
 *  Author: Tomas Baca
 */ 

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <avr/io.h>
#include "config.h"
#include "ioport.h"
#include "csp.h"
#include "csp_thread.h"
#include "csp_clock.h"
#include "csp_if_i2c.h"

// POWER_SWITCH on PORTE.3
#define	POWER_SWITCH			IOPORT_CREATE_PIN(PORTE, 3)
#define	MEM_CS					IOPORT_CREATE_PIN(PORTC, 4)
#define	MEM_WP					IOPORT_CREATE_PIN(PORTC, 3)

#define power_switch_on()		ioport_set_pin_level(POWER_SWITCH, false)
#define power_switch_off()		ioport_set_pin_level(POWER_SWITCH, true)
#define power_switch_toggle()	ioport_toggle_pin_level(POWER_SWITCH)

/* -------------------------------------------------------------------- */
/*	RTC																	*/
/* -------------------------------------------------------------------- */
extern volatile unsigned long milisecondsTimer;
extern volatile unsigned long secondsTimer;
extern volatile unsigned long hoursTimer;

/* -------------------------------------------------------------------- */
/*	Initialize the xMega peripherals									*/
/* -------------------------------------------------------------------- */
void boardInit();
void timerRTC(void *p);
void enable_xtal(void);
void adc_init(void);
unsigned int adca_read_ch0(void);

#endif /* SYSTEM_H_ */