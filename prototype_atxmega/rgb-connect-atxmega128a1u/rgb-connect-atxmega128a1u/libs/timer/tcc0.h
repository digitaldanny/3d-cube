/*
 * tcc0.h
 *
 * Created: 11/12/2018 6:23:06 PM
 *  Author: Daniel Hamilton
 */ 


#ifndef TCC0_H_
#define TCC0_H_


// Initialize TIMER-COUNTER 00000 ON PORT CCCCCC to produce an
// overflow flag once a second (1Hz) and send to event channel 0
void tcc0_init( void );
void config_tcc0_16Mhz ( void );
void tcc0_1Hz ( void );
void tcc0_200Hz ( void );
void tcf0_init ( void );

#endif /* TCC0_H_ */