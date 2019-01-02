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
 *
 *					PORTA(3-0) = GND Layer controls
 *					PORTC(7-0) = LED Pins 15-8
 *					PORTF(7-0) = LED Pins 7-0
 */ 

/*
TODO:
	- Determine pattern memory management...
	- Connect to GUI to determine correct timer/counter settings
	- Create an ON/OFF interrupt to enable and disable the pattern
*/

// headers------------------------------------
#include <avr/io.h>
#include <avr/interrupt.h>
#include "libs/uart/usartd0.h"
#include "libs/timer/tcc0.h"
#include "libs/funcs/rgb-connect-funcs.h"

// globals------------------------------------
volatile uint8_t main_sendNextData = 0x0;
volatile uint8_t patterns[150*8*5];

// global typedefs----------------------------
typedef enum {
	LAYER0,
	LAYER1,
	LAYER2,
	LAYER3
} layer_t;

// prototypes---------------------------------
extern void config_clock( void );

// program code-------------------------------
int main(void)
{
	// module configurations
	config_clock();					// initialize CPU clock to 16 MHz
	config_uart_d0_16Mhz_9600();	// initialize USB uart on port D. CPU clock = 16Mhz, Baud rate = 9600
	config_tcc0_16Mhz();			// initialize TIMER/COUNTER C0 to trigger at a frequency of 30Hz
	config_led_ports();				// initialize output ports to control LEDs
	
	// initialize patterns to 0
	for ( int i = 0; i < sizeof(patterns) - 1; i++ )
	{
		patterns[i] = 0;
	}
	
	// enable interrupts in main code
	PMIC.CTRL = PMIC_HILVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_LOLVLEN_bm;	// enable all level of interrupts
	sei();					
	
	// program executes here
    while (1) 
    {
		if (main_sendNextData)
		{
			// execute repeated patterns here...
			main_sendNextData = 0x0;
		}
    }
}

// Processor response to data being sent over USB.
// Handle storing data for patterns here...
ISR ( USARTD0_RXC_vect )
{
	//...
}

// Trigger boolean that allows main code to send next
// piece of pattern data to the LEDs at a rate of 30Hz
ISR ( TCC0_OVF_vect )
{
	main_sendNextData = 0x1;
}

