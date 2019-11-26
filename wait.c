#include <util/delay_basic.h>
#include "wait.h"

#define LOOPS_PER_MILLIS 4000

/**
 * @brief Wait for the given amount of milliseconds
 * 
 * @param ms the amount of milliseconds to wait
 */
void delay_millis(uint16_t ms) {
	for (uint16_t i = 0; i < ms; ++i)
		_delay_loop_2(LOOPS_PER_MILLIS);
}
