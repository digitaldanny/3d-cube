/*
 * adc.c
 *
 * Created: 11/12/2018 2:10:12 PM
 *  Author: Daniel Hamilton
 */ 

#include "adca.h"

// This function initializes ADC module on PORT AAAAA
// WITH an event channel enabled. ADC input is on PORTA
// pin 4 with 12-bit resolution. Conversion use EVENT CH0.
void config_adc_porta_single_ended( void )
{
	ADCA.REFCTRL = ADC_REFSEL_AREFA_gc;			// 3.3V reference voltage
	ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN4_gc;	// conversion from inputs on pin 4
	ADCA.CH0.CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc; // single ended input

	EVSYS.CH0MUX = (0b1100 << 4) | (0b1 << 3);	// TCC1 triggers event channel 0
	ADCA.EVCTRL = ADC_EVACT_CH0_gc;				// only EVENT CH0 are conversion inputs
	ADCA.CH0.INTCTRL = ADC_CH_INTMODE_COMPLETE_gc | ADC_CH_INTLVL_MED_gc;	// Medium level interrupts for completed conversions

	ADCA.CTRLA = 0x1;	// enable ADCA module
}

// This function is responsible for initializing the Analog-to-Digital
// module on PORT AAAAAA WITHOUT an event channel enabled
// REQUIREMENTS:
//	1. NONE
// USAGE:
//	1. To start a conversion in the main program, use the adca_ch0_start
void adc_init( void )
{
	// ============================= ADCA MODULE CONFIGS ==================================
	// Control register A
	#define DMA_sel		(0x00 << 6)		// no channels selected
	#define CHSTART_sel (0x00 << 2)		// do NOT start channel conversion for CH0
	#define FLUSH_sel	(0x00 << 1)		// all conversions in progress will be aborted and loss
	#define ENABLE_sel	(0x00 << 0)		// do NOT enable the ADC before the rest of the configurations are set
	ADCA.CTRLA = DMA_sel | CHSTART_sel | FLUSH_sel | ENABLE_sel;	// enable the the ADC module and do NOT begin conversion
	
	// Control register B
	#define IMPMODE_sel		(0x00 << 7) // High impedance sources
	#define CURRLIMIT_sel	(0x00 << 5) // NO LIMIT on the sampling rate
	#define CONVMODE_sel	(0x01 << 4)	// the ADC is configured for SIGNED mode
	#define FREERUN_sel		(0x00 << 3)	// not running in freerun mode, so the EVCTRL register isn't swept repeatedly
	#define RESOLUTION_sel	(0x00 << 1)	// 12-BIT RESOLUTION selected, right adjusted
	ADCA.CTRLB = IMPMODE_sel | CURRLIMIT_sel | CONVMODE_sel | FREERUN_sel | RESOLUTION_sel;
	
	// Reference voltage controller
	#define REFSEL_sel		(0x03 << 4) // Reference Voltage = 2V5 reference on PORTB(0)
	#define BANDGAPEN_sel	(0x00 << 1)	// Bandgap disabled
	#define TEMPREF_sel		(0x00 << 0) // Temperature reference disabled
	ADCA.REFCTRL = REFSEL_sel | BANDGAPEN_sel | TEMPREF_sel;
	
	// ============================= ADC CHANNEL 0 CONFIGS ==================================
	
	#define MUXPOS_sel		(0x01 << 3) // positive ADC input is on PIN 1 for differential signal with gain
	#define MUXNEG_sel		(0x02 << 0) // negative ADC input is on PIN 6 for differential signal with gain
	ADCA.CH0.MUXCTRL = MUXPOS_sel | MUXNEG_sel;
	
	// Analog-Digital channel register control
	#define START_sel		(0x00 << 7)	// do not start a conversion on this channel. Conversion will begin when ENABLE_SEL is turned high
	#define GAIN_sel		(0x00 << 2)	// GAIN is equal to 1X
	#define INPUTMODE_sel	(0x02 << 0) // DIFFERENTIAL input signals WITH GAIN
	ADCA.CH0.CTRL = START_sel | GAIN_sel | INPUTMODE_sel;
	
	// ============================== PORT AAAAAAA CONFIGS ===================================
	
	PORTA.DIRCLR = PIN6_bm | PIN1_bm;	// clear pins 6 and pin 1 as inputs for CDS+/-
	ADCA.CTRLA |= PIN0_bm;				// enable the ADC module
}

// This module allows an event on CHANNEL000000 to trigger an interrupt on
// the interrupt vector ADCA_CH0_vect
// REQUIRED:
//	1. adc_init() must be run in the main program before running this function
//	2. SEI() must be run in the main program with the #include <avr/interrupt.h> header
void adc_event_init( void )
{
	PMIC.CTRL |= 0x1;	// low level interrupts enabled
	
	// event channel 0 input configurations
	#define CH0MUX74	(0x0C << 4)	// Timer/Counter C0 event
	#define CH0MUX30	(0x00 << 0)	// Timer/Counter OVF triggers event
	EVSYS.CH0MUX = CH0MUX74 | CH0MUX30;
	
	// output from event channel 0 configurations
	#define SWEEP_sel	(0x00 << 6)	// only event channel 0 is included in the channel sweep
	#define EVSEL_sel	(0x00 << 3) // event channel 0, 1, 2, 3 as inputs
	#define EVACT_sel	(0x01 << 0) // limits the above channels to channel 0
	ADCA.EVCTRL = SWEEP_sel | EVACT_sel | EVACT_sel;
	
	// interrupt configurations		
	#define INTMODE_sel	(0x00 << 2)	// interrupt is triggered when a conversion is complete 
	#define INTLVL_sel	(0x01 << 0) // interrupt level set to LOW level
	ADCA.CH0.INTCTRL = INTMODE_sel | INTLVL_sel;
}

// This function starts an ADC conversion on PORTA, CHANNEL 0 and returns the 16-bit 
// data after the conversion is completed.
// REQUIREMENTS:
//	1. adc_init() must have been run in the main program first
int16_t adca_ch0_conversion( void )
{
	#define START_CH0_CONV_bits (0x1 << 2)
	
	ADCA.CTRLA |= START_CH0_CONV_bits;					// start another conversion on CH0
	while ( (ADCA.INTFLAGS & PIN0_bm) != PIN0_bm );		// wait for conversion on CH0 to complete
	ADCA.INTFLAGS = PIN0_bm;							// reset the interrupt flag again
	
	return ADCA.CH0RES;									// return channel 0 conversion value
}
