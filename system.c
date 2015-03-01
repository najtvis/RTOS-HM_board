/*
 * system.c
 *
 * Created: 11.10.2014 18:40:47
 *  Author: Tomas Baca
 */

 #include "sysclk.h"
 #include "system.h"
 #include "TC_driver.h"
 #include "spi_memory_23lcxx.h"
 #include "clksys_driver.h"
 #include "config.h"
 #include "ADT7420.h"
 
 volatile uint32_t milisecondsTimer;
 volatile uint32_t secondsTimer;
 volatile uint32_t hoursTimer;
 
 unsigned int count_TCD0;

 unsigned int ADD_result;
 
 long signal_offset;
 
/* -------------------------------------------------------------------- */
/*	Initialize the xMega peripherals									*/
/* -------------------------------------------------------------------- */
void boardInit() 
{
	
	// prepare i/o
	ioport_init();
	
	// clock init & enable system clock to all peripheral modules
	sysclk_init();
	
	// enable external oscillator
	enable_xtal();
	
	// enable SPI communication on port C for SPI memory
	spi_mem_init();
	
	sysclk_enable_module(SYSCLK_PORT_GEN, 0xff);
	sysclk_enable_module(SYSCLK_PORT_A, 0xff);
	sysclk_enable_module(SYSCLK_PORT_B, 0xff);
	sysclk_enable_module(SYSCLK_PORT_C, 0xff);
	sysclk_enable_module(SYSCLK_PORT_D, 0xff);
	sysclk_enable_module(SYSCLK_PORT_E, 0xff);
	sysclk_enable_module(SYSCLK_PORT_F, 0xff);
	
	// enable ADC converter
	adc_init();
	
	// init another TWI/I2C interface for ADT (TWIC)
	ADT_init();
		
	/* -------------------------------------------------------------------- */
	/*	Timer for RTC - 1ms													*/
	/* -------------------------------------------------------------------- */
	
	// select the clock source and pre-scaler by 1
	TC1_ConfigClockSource(&TCC1, TC_CLKSEL_DIV1_gc);
	
	TC1_SetOverflowIntLevel(&TCC1, TC_OVFINTLVL_LO_gc);
	
	TC_SetPeriod(&TCC1, 16469);
	
	milisecondsTimer = 0;
	secondsTimer = 0;
	hoursTimer = 0;
	
	/* -------------------------------------------------------------------- */
	/*	Timer for sampling 2kHz												*/
	/* -------------------------------------------------------------------- */
		
	// select the clock source and pre-scaler by 1
	TC0_ConfigClockSource(&TCD0, TC_CLKSEL_DIV1_gc);
		
	TC0_SetOverflowIntLevel(&TCD0, TC_OVFINTLVL_MED_gc);
		
	TC_SetPeriod(&TCD0, 8234);
		
	
	/* -------------------------------------------------------------------- */
	/*	Setup I/Os															*/
	/* -------------------------------------------------------------------- */
	
	ioport_set_pin_dir(POWER_SWITCH, IOPORT_DIR_OUTPUT);
	ioport_set_pin_high(POWER_SWITCH);
}

/* -------------------------------------------------------------------- */
/*	Interrupt for timing the RTC										*/
/* -------------------------------------------------------------------- */
ISR(TCC1_OVF_vect) 
{

	
	// shut down the output PPM pulse

	if (milisecondsTimer++ == 1000) {
		
		milisecondsTimer = 0;
		
		if (secondsTimer++ == 3600) {
			
			secondsTimer = 0;
			hoursTimer++;
		}
	}
	

}

/* -------------------------------------------------------------------- */
/*	Interrupt for sampling input signal	2kHz							*/
/* -------------------------------------------------------------------- */
ISR(TCD0_OVF_vect) 
{

	unsigned long address;
	int adc_value;

	if (sampling == true)
	{

		adc_value =  adca_read_ch0();
		signal_offset += adc_value;
		
		spi_mem_write_word(count_TCD0*2 + MEM_SIGNAL_BEGIN, adc_value);
		
		++count_TCD0;
		if (count_TCD0 == MEM_SIGNAL_LENGTH)
		{
			spi_mem_write_word(MEM_SIGNAL_OFFSET, signal_offset/MEM_SIGNAL_LENGTH);
			power_switch_toggle();
			count_TCD0 = 0;
			sampling = false;
			signal_offset = 0;
		}
		
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


void adc_init(void)
{
// 	struct adc_config adc_conf;
// 	struct adc_channel_config adcch_conf;
// 	adc_read_configuration(&HM_INPUT_ADC, &adc_conf);
// 	adcch_read_configuration(&HM_INPUT_ADC, HM_INPUT_ADC_CH, &adcch_conf);
// 	adc_set_conversion_parameters(&adc_conf, ADC_SIGN_ON, ADC_RES_12, ADC_REF_VCC);		// Vref = VCC / 1.6
// 	adc_set_conversion_trigger(&adc_conf, ADC_TRIG_MANUAL, 1, 0);
// 	adc_set_clock_rate(&adc_conf, 100000UL);
// 	adcch_set_input(&adcch_conf, ADCCH_POS_PIN1, ADCCH_NEG_PIN0, 1);
// 	adc_write_configuration(&HM_INPUT_ADC, &adc_conf);
// 	adcch_write_configuration(&HM_INPUT_ADC, HM_INPUT_ADC_CH, &adcch_conf);
	

	// differential mode
	// negative input is PORTA.0 connected to GND
	
	PORTA.DIR = 0;	 // configure PORTA as input
	ADCA.CTRLA |= 0x1;	 // enable adc
	ADCA.CTRLB = ADC_CONMODE_bm | ADC_RESOLUTION_12BIT_gc;	 // 12 bit conversion, conversion mode - signed - diff
	
	ADCA.REFCTRL = 0x12;	 // internal Vcc/1.6 (3,3/1,6=2,0625V)
	ADCA.PRESCALER = ADC_PRESCALER_DIV128_gc;	 // peripheral clk/128 (16,47MHz ... 128kHz)
	ADCA.CH0.CTRL = ADC_CH_INPUTMODE_DIFF_gc;				  // differential mode, without gain, NEG input PORTA.0!
	ADCA.EVCTRL = 0x1A;
	//ADCA.EVCTRL = 0x01;
	
	
	// in differential mode ****************************
	ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN1_gc | ADC_CH_MUXNEG_PIN0_gc;	// without gain	PIN 1

	_delay_us(10);
	//ADCA.CTRLA |= 0x4;
	// wait for result
	ADCA.CH0.CTRL |= ADC_CH_START_bm;
	while(!ADCA.CH0.INTFLAGS);                           // wait for conversion complete flag
	ADCA.CH0.INTFLAGS=ADC_CH_CHIF_bm;                    // clear int flags (cleared by writing 1)
	
}

unsigned int adca_read_ch0(void)
{
int adc_sum;
unsigned char i;

	adc_sum=0;
	for(i=0;i<4;++i)
	{
		ADCA.CH0.CTRL |= ADC_CH_START_bm; // start conversion on channel 0
		while(!ADCA.CH0.INTFLAGS);
		adc_sum += ADCA.CH0RES;
		//ADCA.CH0.INTFLAGS=ADC_CH_CHIF_bm;                    // clear int flags (cleared by writing 1)
	}
	adc_sum = adc_sum >> 2;
	return adc_sum;
	
}