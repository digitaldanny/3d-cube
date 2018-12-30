/*
* usartd0.h
 *
 * Created: 10/25/2018 6:16:34 PM
 *  Author: Daniel Hamilton
 
 DESCRIPTION: This header file is responsible for declaring UARTD0 methods
 for asynchronous serial communication to COM port from ATXMEGA128A1U uPad
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef USARTD0_H_
#define USARTD0_H_

// INITIALIZE USARTD0 FOR ASYNCHRONOUS SERIAL COMMUNICATION TO PUTTY
//	NOTE: Initialized to run at 57,600 Hz baud rate BSEL = 150
//	PARITY: ODD
void usart_d0_init(void);

// INITIALIZE USARTD0 FOR ASYNCHRONOUS SERIAL COMMUNICATION TO LSM330
//	NOTE: Initialized to run at 115,200 Hz baud rate BSEL = 150
//	PARITY: DISABLED
void usart_d0_lsm330_init(void);

//TRANSMITS A SINGLE BYTE OF DATA
void usart_d0_out_char(char c);

// RECEIVES A SINGLE BYTE OF DATA FROM USARTD0
char usart_d0_in_char(void);

// TRANSMITS A FINITE NULL-TERMINATED STRING OF CHARACTERS
//		NOTE: This function uses 0x0 as the null value
void usart_d0_out_string(char *stringPtr);


#endif /* USARTD0_H_ */