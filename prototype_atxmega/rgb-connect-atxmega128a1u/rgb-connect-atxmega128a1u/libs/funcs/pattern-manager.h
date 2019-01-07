/*
 * pattern_manager.h
 *
 * Created: 1/6/2019 10:01:48 PM
 * Author: Daniel Hamilton
 * Description: These functions manage how all pattern data will be stored
 * after being received from the UART module 
 */ 


#ifndef PATTERN_MANAGER_H_
#define PATTERN_MANAGER_H_

// 16 bits represent LEDs on each layer
// example pattern					L0		L1		L2		L3 (top)
volatile uint16_t pattern0[] = {	0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
									0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0,
									0xFF00, 0xFF00, 0xFF00, 0xFF00,
									0xF000, 0xF000, 0xF000, 0xF000,
									0x0000, 0x0000, 0x0000, 0x0000,
									0xF000, 0xF000, 0xF000, 0xF000,
									0xFF00, 0xFF00, 0xFF00, 0xFF00,
									0xFFF0, 0xFFF0, 0xFFF0, 0xFFF0,
								};
								
volatile uint16_t pattern1[150*8];
volatile uint16_t pattern2[150*8];

// this is the data received from the ISR
volatile uint8_t uart_rxc_array[11];

// Stores all data into the array uart_rxc_array and
// returns a value of 0x1 when the array is full
volatile uint8_t storedCount = 0;
uint8_t storeRxcData( void )
{
	// store received data into uart_rxc_array
	uart_rxc_array[storedCount] = USARTD0.DATA;
	storedCount++;
	
	// return the boolean value if the array has been filled
	if ( storedCount == 11 ) 
	{
		storedCount = 0;
		return 0x1;
	}
	
	// otherwise, return a 0 to allow the array to continue filling
	return 0x0;
}

#endif /* PATTERN_MANAGER_H_ */