/**
 * @file rendering.h
 * @brief Display API
 */

#ifndef __rendering_h
#define __rendering_h

#include <avr/io.h>
#include <stdbool.h>

void init_buffer(void);
void flash_buffer(void);

void clear_buffer(void);
void set_pixel(uint8_t x, uint8_t y, bool active);
void fill_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, bool active);

#endif