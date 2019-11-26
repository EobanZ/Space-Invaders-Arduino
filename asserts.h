/**
 * @file asserts.h
 * @brief Asserts
 */

#ifndef asserts_h
#define asserts_h

#include "assert.h"

#define ERR_DEFAULT 0b11111111
#define ERR_LOOP_EXIT 0b11110000
#define ERR_INIT 0b10000001
#define ERR_ARRAY_FULL 0b11100111
#define ERR_NULL_POINTER 0b00011000
#define ERR_OVERFLOW 0b00000111
#define ASSERT_LED(code, expr) if(expr == 0){DDRF = 0xFF; PORTF = code; while(1);}

#endif
