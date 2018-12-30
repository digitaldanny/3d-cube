/*
 * main.c
 *
 * Created: 12/29/2018 12:13:35 AM
 * Author : Daniel Hamilton
 * Description: This main allows the atxmega128a1u dev board to store bytes of data from a MATLAB GUI 
 *				into internal SRAM. The user can determine to load pattern data to SRAM or to just display
 *				on the LEDs. User can only load a frame of pattern data at a time, not a layer of frame data.
 *				
 * Specifications:	CPU Clock: 16 MHz
 *					UART Baud Rate: 9600
 */ 

#include <avr/io.h>
#include "libs/uart/usartd0.h"

int main(void)
{
	// module configurations
	config_uart_d0_16Mhz_9600();	// initialize USB uart on port D. CPU clock = 16Mhz, Baud rate = 9600
	
	// program executes here
    while (1) 
    {
		
    }
}

