
/*
 * clock.s
 *
 * Created: 11/8/2018 2:11:30 PM
 *  Author: Daniel Hamilton
 */ 

 ; SOURCE CODE MUST INCLUDE FOLLOWING PROTOTYPE ABOVE MAIN PROGRAM
 ;				"extern void clock_init( void  )"

 #include <avr/io.h>
 .global clock_init
 .org 0x100
 clock_init:

	// 1.) allow the protected IO registers to be changed for 4 more clock cycles
	ldi r16, 0xD8
	sts CPU_CCP, r16
	
	// enable an oscillator (OSC_CTRL)
	ldi r16, 0x02			// 32 MHz oscillator
	sts OSC_CTRL, r16

	// wait for the oscillator to stabilize (OSC_STATUS)
	WAIT:
	lds r16, OSC_STATUS					
	SBRS r16, 1		// flag for when 32 MHz is stable
		rjmp WAIT

	// 2.) allow the protected IO registers to be changed for 4 more clock cycles
	ldi r16, 0xD8
	sts CPU_CCP, r16

	// set the oscillator as the new clock source (CLK_CTRL)
	ldi r16, 0x01			// 32 MHz
	sts CLK_CTRL, r16

	// 2.) allow the protected IO registers to be changed for 4 more clock cycles
	ldi r16, 0xD8
	sts CPU_CCP, r16

	// adjust the clock prescaler (CLK_PSCTRL)
	ldi r16, 0b00010100		// division by 8 => 32/8 = 4 MHz
	sts CLK_PSCTRL, r16

 RET