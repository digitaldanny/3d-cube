/*
 * rgb_connect_funcs.h
 *
 * Created: 1/2/2019 12:22:37 AM
 *  Author: Daniel Hamilton
 * Description: These are functions that will be used in the main program "main.c"
 *				To organize the code, the functions have been placed in this separate
 *				header file.
 */ 


#ifndef RGB_CONNECT_FUNCS_H_
#define RGB_CONNECT_FUNCS_H_

// global typedefs----------------------------
typedef enum {	// only 4 layers exist on the current build. Will need to be updated if larger cubes are designed
	LAYER0,
	LAYER1,
	LAYER2,
	LAYER3
} layer_t;

typedef enum {	// allows a maximum of 10 patterns to be generated (NOTE: No memory is alloted for PATTERNS 5-9)
	PATTERN0,
	PATTERN1,
	PATTERN2,
	PATTERN3,
	PATTERN4,
	PATTERN5,
	PATTERN6,
	PATTERN7,
	PATTERN8,
	PATTERN9
} pattern_t;

// symbolic defs------------------------------
#define L15 PIN7_bm		// PORT CCCCCC - Set to 1 so the layer can activate
#define L14 PIN6_bm		// PORT CCCCCC - Set to 1 so the layer can activate
#define L13 PIN5_bm		// PORT CCCCCC - Set to 1 so the layer can activate
#define L12 PIN4_bm		// PORT CCCCCC - Set to 1 so the layer can activatE
#define L11 PIN3_bm		// PORT CCCCCC - Set to 1 so the layer can activate
#define L10 PIN2_bm		// PORT CCCCCC - Set to 1 so the layer can activate
#define L09 PIN1_bm		// PORT CCCCCC - Set to 1 so the layer can activate
#define L08 PIN0_bm		// PORT CCCCCC - Set to 1 so the layer can activate
#define L07 PIN7_bm		// PORT FFFFFF - Set to 1 so the layer can activate
#define L06 PIN6_bm		// PORT FFFFFF - Set to 1 so the layer can activate
#define L05 PIN5_bm		// PORT FFFFFF - Set to 1 so the layer can activate
#define L04 PIN4_bm		// PORT FFFFFF - Set to 1 so the layer can activate
#define L03 PIN3_bm		// PORT FFFFFF - Set to 1 so the layer can activate
#define L02 PIN2_bm		// PORT FFFFFF - Set to 1 so the layer can activate
#define L01 PIN1_bm		// PORT FFFFFF - Set to 1 so the layer can activate
#define L00 PIN0_bm		// PORT FFFFFF - Set to 1 so the layer can activate
#define LYR0 PIN0_bm	// PORT AAAAAA - GND FOR LAYER 0 - Set to 0 so the layer can activate
#define LYR1 PIN1_bm	// PORT AAAAAA - GND FOR LAYER 1 - Set to 0 so the layer can activate
#define LYR2 PIN2_bm	// PORT AAAAAA - GND FOR LAYER 2 - Set to 0 so the layer can activate
#define LYR3 PIN3_bm	// PORT AAAAAA - GND FOR LAYER 3 - Set to 0 so the layer can activate


// Port A activates a specific layer---------------------------------------------------------
layer_t activateLayer( layer_t layer )
{
	layer_t nextLayer;	// tells the main program what the next layer state will be
	
	// activate layer 0 and disable all other layers
	if ( layer == LAYER0 )
	{
		PORTA.OUTSET = LYR1 | LYR2 | LYR3;
		PORTA.OUTCLR = LYR0;
		nextLayer = LAYER1;
	}
	
	// activate layer 1 and disable all other layers
	else if ( layer == LAYER1 )
	{
		PORTA.OUTSET = LYR0 | LYR2 | LYR3;
		PORTA.OUTCLR = LYR1;
		nextLayer = LAYER2;
	}
	
	// activate layer 2 and disable all other layers
	else if ( layer == LAYER2 )
	{
		PORTA.OUTSET = LYR0 | LYR1 | LYR3;
		PORTA.OUTCLR = LYR2;
		nextLayer = LAYER3;
	}
	
	// activate layer 3 and disable all other layers
	else if ( layer == LAYER3 )
	{
		PORTA.OUTSET = LYR0 | LYR1 | LYR2;
		PORTA.OUTCLR = LYR3;
		nextLayer = LAYER0;
	}
	
	return nextLayer;
};

// enable interrupts in main code
void config_interrupts( void )
{
	PMIC.CTRL = PMIC_HILVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_LOLVLEN_bm;	// enable all level of interrupts
	sei();
}

// CONFIGURE LED PORTS-----------------------------------------------------------------------
void config_led_ports( void )
{
	// port configurations - at least 20 output signals are required to control all 64 LEDs
	PORTA.DIRSET = 0x0F;	// Controls which layers are able to turn on
	PORTC.DIRSET = 0xFF;	// Port C controls L15 - L8
	PORTF.DIRSET = 0xFF;	// Port F controls L7 - L0
	
	// initialize layers to be off
	PORTA.OUTSET = LYR0 | LYR1 | LYR2 | LYR3;						// 1's turn layers off
	PORTC.OUTCLR = L15 | L14 | L13 | L12 | L11 | L10 | L09 | L08;	// 0's turn LEDs off
	PORTF.OUTCLR = L07 | L06 | L05 | L04 | L03 | L02 | L01 | L00;	// 0's turn LEDs off
}


#endif /* RGB_CONNECT_FUNCS_H_*/