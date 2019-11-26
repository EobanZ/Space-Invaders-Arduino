#include <avr/io.h>
#include <stdbool.h>

#include "lcd.h"
#include "rendering.h"

#define SCREEN_SIZE (128 * 8)
#define _IDX(chip, page, addr) ((chip) * 512 + (page) * 64 + (addr))
#define _POS(x, y) _IDX((x) >= 64, (int)((y) / 8), (x) % 64)

/**
 * Screen array layout is as follows:
 *
 * the first half is the first chip, the second half is the second chip,
 * so all pixels of the left hand side of the screen are stored at indices
 * 0-511 and the right hand side at 512-1023.
 *
 * Each half is separated in 8 blocks of 64 integers, each being one column
 * of one of the 8 pages.
 *
 * The 8 page blocks are arranged just as you'd think, first page 0, then 1, etc.
 *
 * The array is stored that way because when flashing the screen, this is the way
 * the values will be accessed, giving a better performance due to memory prefetching.
 * 
 */
uint8_t screen[SCREEN_SIZE];

/**
 * @brief Initializes screen communication and screen buffer. Call once at the beginning
 */
void init_buffer(void) {
    clear_buffer();

    // initialize data and control ports to be output on all pins
    DDRC = 0xFF;
    DDRA = 0xFF;
    DDRF = 0xFF;

    set_reset(false);
    chipselect_1(true);
    chipselect_2(true);
    display_on();
    chipselect_2(false);
}

/**
 * @brief Flashes the content of the buffer to the lcd
 */
void flash_buffer(void) {
    for (int chip = 0; chip < 2; ++chip) {
        chipselect(chip);

        for (int page = 0; page < 8; ++page) {
            set_page(page);

            for (int addr = 0; addr < 64; ++addr) {
                set_address(addr);
                send_data(screen[_IDX(chip, page, addr)]);
            }
        }
    }
}

/**
 * @brief clears the buffer, setting all values to zero
 */
void clear_buffer(void) {
    for (int i = 0; i < SCREEN_SIZE; ++i) {
        screen[i] = 0;
    }
}

/**
 * @brief Sets the value of a pixel in the screen buffer to either on of off, depending on the active param
 * 
 * @param x the x position of the pixel
 * @param y the y position of the pixel
 * @param active true if the pixel should be white, false otherwise
 */
void set_pixel(uint8_t x, uint8_t y, bool active) {
    x %= 128;
    y %= 64;

    if (active) screen[_POS(x, y)] |= (1 << (y % 8));
    else screen[_POS(x, y)] &= ~(1 << (y % 8));
}

/**
 * @brief Fills a rectangle, setting all pixels to either on or off, depending on the active param
 * 
 * @param _x the x position of the rect
 * @param _y the y position of the rect
 * @param width the width of the rect
 * @param height the height of the rect
 * @param active true if the pixel should be white, false otherwise
 */
void fill_rect(uint8_t _x, uint8_t _y, uint8_t width, uint8_t height, bool active) {
    for (uint8_t x = 0; x < width; ++x) {
        for (uint8_t y = 0; y < height; ++y) {
            set_pixel(_x + x, _y + y, active);
        }    
    }
}