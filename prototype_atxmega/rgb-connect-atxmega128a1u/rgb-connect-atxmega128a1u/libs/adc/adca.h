/*
 * adc.h
 *
 * Created: 11/12/2018 2:08:49 PM
 *  Author: Daniel Hamilton
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>

// ADC MODULE ON PORT A PROTOTYPES
void config_adc_porta_single_ended( void );
void adc_init( void );
void adc_event_init( void );
int16_t adca_ch0_conversion( void );

#endif /* ADC_H_ */