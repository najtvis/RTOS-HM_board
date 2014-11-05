/*
 * config.h
 *
 * Created: 11.10.2014 18:31:37
 *  Author: Tomas Baca
 */ 

#ifndef CONFIG_H_
#define CONFIG_H_

#include "twi_master_driver.h"

/* -------------------------------------------------------------------- */
/*	Definitions for HM board											*/
/* -------------------------------------------------------------------- */

// Virtual timers in HM board
#define NUM_TIMERS			1

// Sampling rate input signal
#define SAMPLING_RATE		2000

// constants for FFT calculating
#define SIGNAL_BEGINING					0
#define FFT_SIGNAL_ADDRESS				0
#define NO_POINTS						512				// numbers of point to calculate by FFT, number must be power of 2!
#define DECIMATE_TO_FREQUENCY			500
#define DECIMATION_FACTOR				SAMPLING_RATE/DECIMATE_TO_FREQUENCY;

// constants for least mean square calculation
#define NO_AVERAGING_POINTS				300
#define BEGINING_AVERAGING_OFFSET		200				// begining averaging offset from highest point of envelope
#define LEAST_MEAN_SQUARE_POINTS		1000

// addresses position in SPI memory
#define BEGIN_SAMPLED_SIGNAL_ADDRESS	0
#define BEGIN_FFT_RESULT_ADDRESS		NO_POINTS*8		// 2xlong - real and complex value
#define BEGIN_LEAST_SQUARE_ADDRESS		LEAST_MEAN_SQUARE_POINTS * 2 + BEGIN_FFT_RESULT_ADDRESS 



/* -------------------------------------------------------------------- */
/*	CSP define															*/
/* -------------------------------------------------------------------- */
// Address of the local CSP node
#define CSP_MY_ADDRESS	4		// Deska HM

// Address of the OBC CSP node
#define CSP_OBC_ADDRESS	1	

// Actual position of CSP i2c on the xMega
// Replace "TWIE" with adequate port of your board
#define CSP_I2C_INTERFACE	TWIE
#define CSP_I2C_TWIM		TWIE_TWIM_vect
#define CSP_I2C_TWIS		TWIE_TWIS_vect

// Slave address of my CSP i2c interface
#define CSP_I2C_SLAVE_ADDRESS	CSP_MY_ADDRESS

// Slave address of the OBC CSP i2c interface
#define CSP_I2C_OBC_ADDRESS	CSP_OBC_ADDRESS

// Number of CSP buffer to allocate
#define CSP_BUFFER_COUNT	1

// CSP Buffer size in bytes
#define CSP_BUFFER_SIZE	86

// Maximum size of CSP packet, payload + header
#define CSP_PACKET_SIZE	72

// Size of CSP router stack in words
#define CSP_ROUTER_STACK	500

// Do not ever change!!!
#define CSP_ROUTER_PRIORITY	0

// Baudrate of the CSP i2c interface
#define CSP_I2C_BAUDRATE	400000
#define CSP_I2C_BAUDSETTING TWI_BAUD(F_CPU, CSP_I2C_BAUDRATE)

#define SPI_MEM_INTERFACE SPIC



#endif /* CONFIG_H_ */


