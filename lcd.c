#include <avr/io.h>
#include <stdbool.h>
#include <util/delay_basic.h>

#include "lcd.h"
#include "wait.h"
#include "asserts.h"

bool displayOn = false;

/**
 * @brief Toggles the clock signal needed for synchronization of the lcd
 */
void clock_toggle(void) {
    if (!TIMER_SIGNAL) PORTC |= bit(DDC5);
    else PORTC &= ~bit(DDC5);
}

/**
 * @brief set the bus to data mode
 */
void set_data_input(void) {
    PORTC |= bit(DDC7);
}

/**
 * @brief set the bus to instruction mode
 */
void set_instruction_input(void) {
    PORTC &= ~bit(DDC7);
}

/**
 * @brief set the data mode to write
 */
void set_data_write(void) {
    PORTC &= ~bit(DDC6);
}

/**
 * @brief set the data mode to read
 */
void set_data_read(void) {
    PORTC |= bit(DDC6);
}

/**
 * @brief set the reset bit
 */
void set_reset(bool val) {
    if (!val) PORTC |= bit(DDC2);
    else PORTC &= ~bit(DDC2);
}

/**
 * @brief set the data on the data bus
 */
void set_data(uint8_t data) {
    PORTA = data;
}

/**
 * @brief select the first chip controlling the left half of the screen
 */
void chipselect_1(bool val) {
    if (val) PORTC |= bit(DDC4);
    else PORTC &= ~bit(DDC4);
}

/**
 * @brief select the second chip controlling the right half of the screen
 */
void chipselect_2(bool val) {
    if (val) PORTC |= bit(DDC3);
    else PORTC &= ~bit(DDC3);
}

/**
 * @brief select the given chip
 */
void chipselect(uint8_t chip) {
    chipselect_1(chip == 0);
    chipselect_2(chip == 1);
}

/**
 * @brief flash the data or instruction on the bus to the lcd
 */
void flash_data(void) {
    _delay_loop_2(TIMER_DELAY);
    clock_toggle();
    _delay_loop_2(TIMER_DELAY);
    clock_toggle();
    _delay_loop_2(TIMER_DELAY);
}

/**
 * @brief send an instruction to the lcd
 * 
 * @param instr the instruction
 */
void send_instruction(uint8_t instr) {
    set_data_write();
    set_instruction_input();
    set_data(instr);
    flash_data();
}

/**
 * @brief send data to the lcd
 * 
 * @param data the data
 */
void send_data(uint8_t data) {
    set_data_write();
    set_data_input();
    set_data(data);
    flash_data();
}

/**
 * @brief turn the display on
 * 
 * Note, that this will only turn on selected chips, so in order to turn on the whole screen,
 * call chipselect_1(true) and chipselect_2(true) beforehand
 */
void display_on(void) {
    send_instruction(OP_DISPLAY_ON);
    clear_screen();
}

/**
 * @brief set the column in the current page
 * 
 * @param addr the column, from 0 to 63
 */
void set_address(uint8_t addr) {
    ASSERT_LED(ERR_OVERFLOW, (addr < 64));
    
    uint8_t instr = (0b01 << 6) | (addr & 0b111111);
    send_instruction(instr);
}

/**
 * @brief select the page
 * 
 * @param page the numer of the page, from 0 to 7
 */
void set_page(uint8_t page) {
    ASSERT_LED(ERR_OVERFLOW, (page < 8));

    uint8_t instr = (0b10111 << 3) | (page & 0b111);
    send_instruction(instr);
}

/**
 * @brief clears the given page
 * 
 * @param page the numer of the page, from 0 to 7
 */
void clear_page(uint8_t page) {
    set_page(page);
    for (int i = 0; i < 64; ++i) {
        set_address(i);
        send_data(0);
    }
}

/**
 * @brief clears the whole screen
 */
void clear_screen(void) {
    for (int p = 0; p < 8; ++p) {
        clear_page(p);
    }
}