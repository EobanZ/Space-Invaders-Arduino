/**
 * @file combat.h
 * @brief Shooting logic.
 */


#ifndef __combat_h
#define __combat_h

#include <avr/io.h>

/**
 * @brief A laser ray is moving downwards (shot by a monster)
 */
#define DIRECTION_DOWN 1

/**
 * @brief A laser ray is moving upwards (shot by a monster)
 */
#define DIRECTION_UP -1

/**
 * @brief Representing an object which can be hit by a laser ray.
 * @param x X Coordinate
 * @param y Y Coordinate
 * @param width Width of the object in pixels
 * @param height Height of the object in pixels
 * 
 */
typedef struct {
    int8_t x, y, width, height;
} collider;

uint8_t register_collider(collider* c);
void remove_collider(uint8_t index);
void register_collide_callback(void (*func)(uint8_t));

void shoot_laser(uint8_t x, uint8_t y, int8_t direction);
void animate_lasers(void);
void render_lasers(void);
void reset_combat(void);

#endif