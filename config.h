/*
 * config.h
 *
 * Created: 11.10.2014 18:31:37
 *  Author: Tomas Baca
 */ 

#ifndef CONFIG_H_
#define CONFIG_H_

#include "twi_master_driver.h"
#include "adc.h"

// Address of the local CSP node
// #define CSP_MY_ADDRESS	2	// Deska EPS
// #define CSP_MY_ADDRESS	3	// Deska Measure
#define CSP_MY_ADDRESS	6		// Deska HM

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
#define CSP_BUFFER_COUNT	3

// CSP Buffer size in bytes
#define CSP_BUFFER_SIZE	86

// Maximum size of CSP packet, payload + header
#define CSP_PACKET_SIZE	72

// Size of CSP router stack in words
#define CSP_ROUTER_STACK	255

// Do not ever change!!!
#define CSP_ROUTER_PRIORITY	1

// Baudrate of the CSP i2c interface
#define CSP_I2C_BAUDRATE	100000
#define CSP_I2C_BAUDSETTING TWI_BAUD(F_CPU, CSP_I2C_BAUDRATE)

#define SPI_MEM_INTERFACE	SPIC
#define SPI_PORT			PORTC

#define HM_INPUT_ADC		ADCA
#define HM_INPUT_ADC_CH		ADC_CH0

#define ADT_I2C_INTERFACE	TWIC
#define ADT_I2C_TWIM		TWIC_TWIM_vect
#define ADT_I2C_BAUDRATE	100000
#define ADT_I2C_BAUDSETTING TWI_BAUD(F_CPU, ADT_I2C_BAUDRATE)
#define ADT_I2C_ADDRESS		0x48

// SPI memory address allocation (according to document "SPI memory allocation.xlsx")
#define MEM_SIGNAL_BEGIN			0x000000		// MEM_SIGNAL_LENGTH*2	(4kB)
#define MEM_ATTENUATION_BEGIN		0x001000		// Signal decimated to 1kHz (2kB)
#define MEM_FFT_BEGIN				0x001800		// 8192 point * 8B = 64kB for FFT (2xFloat)
#define MEM_SIGNAL_OFFSET			0x011800		// stored signal offset of actual signal (2B - int)

#define MEM_SIGNAL_LENGTH			2048			// at 2kHz is length 1,028s

#define NO_POINTS					512      // numbers of point to calculate by FFT, number must be power of 2!
#define SAMPLING_RATE				2000
#define DECIMATE_TO_FREQUENCY		1000



// constants for least mean square calculation
#define NO_AVERAGING_POINTS			300

bool sampling;		// if sampling is true - enabled ADC
#endif /* CONFIG_H_ */