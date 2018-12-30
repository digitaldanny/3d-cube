/*
 * main.c
 *
 * Created: 12/29/2018 12:13:35 AM
 * Author : Daniel Hamilton
 * Description: This main allows the atxmega128a1u dev board to store bytes of data from a MATLAB GUI 
 *				into internal SRAM. The user can determine to load pattern data to SRAM or to just display
 *				on the LEDs. User can only load a frame of pattern data at a time, not a layer of frame data.
 *				
 * Specifications:	CPU Clock: 2 MHz
 *					UART Baud Rate: 9600
 */ 

#include <avr/io.h>
#include "usartd0.h"

// GUI interfacing module
int main(void)
{
    while (1) 
    {
		
    }
}