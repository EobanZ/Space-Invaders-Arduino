#include "input.h"

/**
 * @brief Initialize the Input ADC
 * 
 */
void init_input(void)
{
    init_ADC();
}

/**
 * @brief Get the x and y input from the definend ADC pin
 * 
 * @param x Input on the x axis
 * @param y Input on the y axis
 */
void get_input(int16_t* x, int16_t* y)
{
    *x = read_ADC(JOY_X);
    *y = read_ADC(JOY_Y);
}