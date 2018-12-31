/*
 * usartd0.c
 *
 * Created: 11/5/2018 5:15:12 PM
 *  Author: Daniel
 */ 

#include "usartd0.h"

// Initialize UART module for asynch serial comm to com port for transferring
// data between GUI and processor. CPU clock = 16 Mhz, Baud rate = 9600
// Uses odd parity, 1 stop bit, and data size of 8 bits.
void config_uart_d0_16Mhz_9600(void)
{
	#define BSEL_VAL 1651			// Bottom byte of the BSEL value
	#define BSCALE_VAL 0xC			// Top nibble is the BSCALE value, bottom nibble is the rest of the BSEL
	
	cli();
	USARTD0.CTRLA		= USART_RXCINTLVL_HI_gc;							// Turn on the0 RXCINT so received data triggers ISR in main 
	USARTD0.CTRLB		= 0b00011000;										// TRANSMIT enabled = bit 3, RECIEVE enabled = bit 4
	USARTD0.BAUDCTRLA	= (BSEL_VAL & 0xFF);								// Initialize the first nibble of the BAUD RATE (11:8) and the BSCALE (3:0) value
	USARTD0.BAUDCTRLB	= ((BSCALE_VAL << 4) & 0xF0) | ( (BSEL_VAL >> 8) & 0x0F );	// Initialize the BAUD RATE (7:0)
	USARTD0.CTRLC		= 0b00110011;										// Protocol configs - 00 (async) 11 (odd parity) 0 (1 stop bit) 011 (data size is 8 bits)
	PORTD.DIRSET		= 0b01111000;										// Initialize pin 3 as an output for TRANSMITTING DATA and pin 2 as an INPUT for RECEIVING DATA
	PORTD.OUTSET		= 0b01111000;										// Initialize
	sei();	
}

// INITIALIZE USARTD0 FOR ASYNCHRONOUS SERIAL COMMUNICATION TO PUTTY
//	NOTE: Initialized to run at 57,600 Hz baud rate BSEL = 150
//	PARITY: ODD
void usart_d0_init(void)
{
	#define BSEL_VAL 11				// Bottom byte of the BSEL value
	#define BSCALE_VAL 0x90			// Top nibble is the BSCALE value, bottom nibble is the rest of the BSEL
	
	cli();
	USARTD0.CTRLA		= 0b00010000;		// Turn on the DREINT and RXCINT so my interrupts can be triggered
	USARTD0.CTRLB		= 0b00011000;		// TRANSMIT enabled = bit 3, RECIEVE enabled = bit 4
	USARTD0.BAUDCTRLA	= BSEL_VAL;			// Initialize the first nibble of the BAUD RATE (11:8) and the BSCALE (3:0) value
	USARTD0.BAUDCTRLB	= BSCALE_VAL;		// Initialize the BAUD RATE (7:0)
	USARTD0.CTRLC		= 0b00110011;		// Protocol configs - 00 (async) 11 (odd parity) 0 (1 stop bit) 011 (data size is 8 bits)
	PORTD.DIRSET		= 0b01111000;		// Initialize pin 3 as an output for TRANSMITTING DATA and pin 2 as an INPUT for RECEIVING DATA
	PORTD.OUTSET		= 0b01111000;		// Initialize
	sei();
}

// INITIALIZE USARTD0 FOR ASYNCHRONOUS SERIAL COMMUNICATION TO LSM330
//	NOTE: Initialized to run at 115,200 Hz baud rate BSEL = 150
//	PARITY: DISABLED
void usart_d0_lsm330_init(void)
{
	#define BSEL_VAL 11				// Bottom byte of the BSEL value
	#define BSCALE_VAL 0x90			// Top nibble is the BSCALE value, bottom nibble is the rest of the BSEL
	
	cli();
	USARTD0.CTRLA		= 0b00010000;		// Turn on the DREINT and RXCINT so my interrupts can be triggered
	USARTD0.CTRLB		= 0b00011000;		// TRANSMIT enabled = bit 3, RECIEVE enabled = bit 4
	USARTD0.BAUDCTRLA	= BSEL_VAL;			// Initialize the first nibble of the BAUD RATE (11:8) and the BSCALE (3:0) value
	USARTD0.BAUDCTRLB	= BSCALE_VAL;		// Initialize the BAUD RATE (7:0)
	USARTD0.CTRLC		= 0b00000011;		// Protocol configs - 00 (async) 11 (odd parity) 0 (1 stop bit) 011 (data size is 8 bits)
	PORTD.DIRSET		= 0b01111000;		// Initialize pin 3 as an output for TRANSMITTING DATA and pin 2 as an INPUT for RECEIVING DATA
	PORTD.OUTSET		= 0b01111000;		// Initialize
	sei();
}

//TRANSMITS A SINGLE BYTE OF DATA
void usart_d0_out_char(char c)
{
	while ( (USARTD0.STATUS&0x20) != 0x20 );	// Wait until the DREIF flag (bit 5) is set
	USARTD0.DATA = c;							// Store the new data into the data IO to send when the FIFO is ready
}

// RECEIVES A SINGLE BYTE OF DATA FROM USARTD0
char usart_d0_in_char(void)
{
	char in_char;
	
	while ( (USARTD0.STATUS&0x80) != 0x80 );	// Wait until the RXCIF flag (bit 7) is set
	in_char = USARTD0.DATA;
	return in_char;								// Return the new data from the IO
}

// TRANSMITS A FINITE NULL-TERMINATED STRING OF CHARACTERS
//		NOTE: This function uses 0x0 as the null value
void usart_d0_out_string(char *stringPtr)
{
	#define nullVal 0x0
	char nextChar = *stringPtr;
	
	do
	{
		usart_d0_out_char(nextChar);		// Transmit the next character of the string through UART
		nextChar = *stringPtr++;			// Load data at the next address
	}
	while ( nextChar != nullVal );			// Transmit characters until null value is reached
}