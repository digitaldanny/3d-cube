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
	- More than 3 patterns causes a memory overflow - may need to use external SRAM
	- Create *.c file for rgb-connect-funcs.h header file
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
#include "libs/adc/adca.h"
#include "libs/funcs/pattern-manager.h"

// pattern size must be updated through UART receiver interrupt
volatile uint8_t p0s = 4*8;			
volatile uint8_t p1s = 1;
volatile uint8_t p2s = 1;

volatile uint16_t selectedPattern[150*8];	// selected pattern has a max size of 150 frames where 1 frame is 8 bytes
volatile uint8_t nextPatternSize;			// will be updated to the size of the currently selected pattern

// globals------------------------------------
volatile uint8_t main_sendNextLayer = 0x0;				// boolean to allow next layer to show on cube
volatile uint8_t main_sendNextFrame = 0x0;				// boolean to allow next frame to display on the cube
volatile layer_t nextLayer = LAYER0;					// resets to 0 after 4 iterations
volatile pattern_t nextPattern = PATTERN0;				// currently selected pattern to play
volatile uint8_t layerCounter = 0;						// resets to 0 after completing the requested pattern
volatile int framesAvailable;							// number of available frames in the animation
volatile int frameCount = 0;							// current frame number cycling
volatile int potDelay = 5;								// default frame delay is 5 TCF0 cycles

// prototypes---------------------------------
extern void config_clock( void );

// program code-------------------------------
int main(void)
{
	// module configurations
	config_adc_porta_single_ended(); // initialize ADCA module to create create conversion when TCC1 OVF	(libs/adc/adca.h)
	config_clock();					// initialize CPU clock to 16 MHz										(clock.s)
	config_uart_d0_16Mhz_9600();	// initialize USB uart on port D. CPU clock = 16Mhz, Baud rate = 9600	(libs/uart/usard0.h)
	config_tcc0_16Mhz();			// initialize TIMER/COUNTER C0 to trigger at a frequency of 240Hz		(libs/timer/tcc0.h)	
	tcf0_init();					// initialize TIMER/COUNTER F0 to trigger at a frequency of 30Hz		(libs/timer/tcc0.h)
	tcc1_init();					// initialize TIMER/COUNTER C1 to trigger at a frequency of 10Hz		(libs/timer/tcc0.h)
	config_led_ports();				// initialize output ports to control LEDsl								(libs/funcs/rgb-connect-funcs.h)
	config_interrupts();			// enable interrupts													(libs/funcs/rgb-connect-funcs.h)				
	
	// initialize pattern memory to 0's
	for ( int i = 0; i < nextPatternSize - 1; i++ ) selectedPattern[i] = 0;	
	
	// program executes here
    while (1) 
    {
		
		// create a way for the pattern to be updated by a user through some interrupt----------------------
		switch( nextPattern )
		{

			case PATTERN0:
				nextPatternSize = p0s;
				for (int i = 0; i < nextPatternSize; i++) selectedPattern[i] = pattern0[i];
				break;

			case PATTERN1:
				nextPatternSize = p1s;
				for (int i = 0; i < nextPatternSize; i++) selectedPattern[i] = pattern1[i];
				break;

			case PATTERN2:
				nextPatternSize = p2s;
				for (int i = 0; i < nextPatternSize; i++) selectedPattern[i] = pattern2[i];
				break;

			default: // PATTERNS 3-9
				break;

		}
		framesAvailable = nextPatternSize / 4;

		// execute repeated patterns--------------------------------------------------------------------
		if (main_sendNextLayer)
		{

			nextLayer = activateLayer(nextLayer);							// turns on the current layer and returns the next layer that should be activated (rgb-connect-funcs.h)
			PORTC.OUT = (selectedPattern[ layerCounter ] >> 8) & 0xFF;		// LEDs 15-8 are activated with PORTC and the top byte of pattern data
			PORTF.OUT = selectedPattern[ layerCounter ] & 0xFF;				// LEDs 7-0 are activated with PORTF and the bottom byte of pattern data
			
			// finish the frame by going to the next layer
			layerCounter++;														// go to the next layer 
			if (layerCounter % 4 == 0 && layerCounter > 0) layerCounter -= 4;	// restart frame at bottom layer if the end of the frame is reached

			main_sendNextLayer = 0x0;										// reset boolean so data displays until next timer interrupt
		}

		// play the next frame if the TCC1 is triggered and at the first layer of the frame-------------
		if (main_sendNextFrame && nextLayer == LAYER0)
		{
			if ( frameCount < framesAvailable - 1 )
			{
				frameCount++;		// go to the next frame
				layerCounter += 4;	// go to next 64 LED values
			} 
			
			else 
			{
				frameCount = 0;		// restart from beginning of animation
				layerCounter = 0;	// restart from beginning of animation
			}
			
			main_sendNextFrame = 0x0;			// reset boolean so the next frame isn't set
			TCC0.INTCTRLA |= ( 0x01 << 0 );		// enable TCC0 layer interrupt again
			TCF0.INTCTRLA |= ( 0x01 << 0 );		// enable TCF0 frame interrupt again
		}
		else									// if I don't need to go to the next frame, allow more interrupts
			TCC0.INTCTRLA |= ( 0x01 << 0 );		// enable TCC0 interrupt again until reaching beginning of frame
    }
}

// Processor response to data being sent over USB.
// Handle storing data for patterns here...
// HIGH LEVEL
ISR ( USARTD0_RXC_vect )
{
	volatile uint8_t full = storeRxcData();
	if (full)
	{
		full = 0x0;
	}
}

// Trigger boolean that allows main code to send next
// layer of pattern data to the LEDs at a rate of 60*4 = 240 Hz
// LOW LEVEL
ISR ( TCC0_OVF_vect )
{
	main_sendNextLayer = 0x1;
	TCC0.INTCTRLA &= ~( 0x01 << 0 ); // disable TCC0 interrupt until handled by main
}

// Input frequency from user that the next pattern should be selected
// MEDIUM LEVEL
ISR ( ADCA_CH0_vect )
{
	potDelay = ADCA.CH0RES / (16*16);	// reduce to lower resolution so there are fewer delay options
	TCC1.INTCTRLA |= ( 0x01 << 0 );		//  enable TCC1 interrupt
}

// TCC1 interrupt is event channel 0's input to start conversion for ADCA module
ISR ( TCC1_OVF_vect )
{
	TCC1.INTCTRLA &= ~( 0x01 << 0 ); // disable TCC1 until ADC conversion is complete
}

// Trigger allows the next pattern data to be send
// at a rate specified by the potentiometer
// MED LEVEL
volatile int TCF0_delay = 0;
ISR ( TCF0_OVF_vect )
{
	if (TCF0_delay > potDelay)			// potDelay is adjusted by ADC module by turning the potentiometer
	{
		main_sendNextFrame = 0x1;
		TCC0.INTCTRLA &= ~( 0x01 << 0 ); // disable TCC0 interrupt until handled by main
		TCF0.INTCTRLA &= ~( 0x01 << 0 ); // disable TCF0 interrupt until handled by main
		TCF0_delay = 0;
	}
	else
		TCF0_delay++;
}

