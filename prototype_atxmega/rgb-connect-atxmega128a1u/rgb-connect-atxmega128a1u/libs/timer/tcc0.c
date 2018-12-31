/*
 * tcc0.c
 *
 * Created: 11/12/2018 6:21:26 PM
 *  Author: Daniel Hamilton
 */ 

#include <avr/io.h>
#include "tcc0.h"

// Initialize TIMER-COUNTER 00000 ON PORT CCCCCC to produce an
// overflow flag once a second (200 Hz) and send to event channel 0
void tcc0_init( void )
{
	// Overflow will be triggered every 1 second
	TCC0.CTRLA = 0x07;		// CLK / 1024								// CONFIGS FOR 1Hz OVF INTERRUPT
	TCC0.PER = 1950;		// 1950 x 1024 / 2M = 1s (1Hz)			    // CONFIGS FOR 1Hz OVF INTERRUPT
	
	// Event action controls
	#define EVACT_sel	(0x00 << 5)		// Event action disabled
	#define EVDLY_sel	(0x00 << 4)		// Event delay by 1 clock cycle is disabled
	#define EVSEL_sel	(0x08 << 0)		// Event channel 0 is used to the overflow
	TCC0.CTRLD = EVACT_sel | EVDLY_sel | EVSEL_sel;
	
	// enable low level interrupts
	#define ERRINTLVL_sel	(0x00 << 2)		// Timer error interrupt level OFF
	#define OVFINTLVL_sel	(0x01 << 0)		// Overflow interrupt level LOW
	TCC0.INTCTRLA = ERRINTLVL_sel | OVFINTLVL_sel;
}

// Configure 30 Hz clock for a 16MHz CPU clock speed
void config_tcc0_16Mhz ( void )
{
	tcc0_init();
	TCC0.CTRLA = 0x07;		// CLK / 1024
	TCC0.PER = 16000000 / (1024 * 30);	
}

void tcc0_1Hz ( void )
{
	// Overflow will be triggered every 1 second
	TCC0.CTRLA = 0x07;		// CLK / 1024								// CONFIGS FOR 1Hz OVF INTERRUPT
	TCC0.PER = 1950;		// 1950 x 1024 / 2M = 1s (1Hz)				// CONFIGS FOR 1Hz OVF INTERRUPT
}

void tcc0_200Hz ( void )
{
	// Overflow will be triggered at a rate of 200 Hz
	TCC0.CTRLA = 0x04;		// CLK / 8								   // CONFIGS FOR 200 Hz OVF INTERRUPT
	TCC0.PER = 1250;		// PER required for 200 Hz				   // CONFIGS FOR 200 Hz OVF INTERRUPT	
}
