/**
 * @file lcd.h
 * @brief Display driver
 */

#define LCD_BUSY 7
#define TIMER_DELAY 10

#define OP_DISPLAY_ON  0b00111111

#define TIMER_SIGNAL ((PORTC >> DDC5) & 1)
#define bit(x) (1 << x)

void clock_toggle(void);
void set_data_input(void);
void set_instruction_input(void);
void set_data_write(void);
void set_data_read(void);
void set_reset(bool val);
void set_data(uint8_t data);
void chipselect_1(bool val);
void chipselect_2(bool val);
void chipselect(uint8_t chip);
void flash_data(void);
void send_instruction(uint8_t instr);
void send_data(uint8_t data);
void display_on(void);
void set_address(uint8_t addr);
void set_page(uint8_t page);
void clear_page(uint8_t page);
void clear_screen(void);