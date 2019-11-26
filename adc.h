#ifndef adc_h
#define adc_h
#include <avr/io.h>
#include <util/delay_basic.h>
#include "asserts.h"



void init_ADC(void);
uint16_t read_ADC(uint8_t channel);


#endif