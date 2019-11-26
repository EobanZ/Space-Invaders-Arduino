/**
 * @file input.h
 * @brief Handling the Input via ADC
 */

#ifndef input_h
#define input_h

#include <avr/io.h>
#include "adc.h"
#include <stdbool.h>

//ADC Pins
/**
 * @brief The Pin which is connected to the X-Axis data of the Joystick.
 */
#define JOY_X 8 

/**
 * @brief The Pin which is connected to the Y-Axis data of the Joystick.
 */
#define JOY_Y 9

void init_input(void);
void get_input(int16_t* x, int16_t* y); 

#endif