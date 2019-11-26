#include "asserts.h"
#include "combat.h"
#include "rendering.h"
#include "sprites.h"
#include <string.h>

// enable to show boxes where colliders are
//#define DEBUG_COLLISSION

#define MAX_LASERS 32
#define MAX_COLLIDERS 32
#define MAX(x, y) (x > y ? x : y)

/**
 * @brief Representing a Laser Ray.
 * 
 */
typedef struct {
    uint8_t x;
    uint8_t y;
    int8_t dir;
    bool deleted;
} laser;

laser lasers[MAX_LASERS];
uint8_t count_lasers = 0;

collider *colliders[MAX_COLLIDERS];
uint8_t count_colliders = 0;
bool collider_deleted[MAX_COLLIDERS];
void (*callback)(uint8_t);

/**
 * @brief Register a function to handle a collision
 * 
 * To set the function which is executed if a collision is detected
 * 
 * @param func the function with signature void func(uint8_t)
 */
void register_collide_callback(void (*func)(uint8_t)) {
    callback = func;
}

/**
 * @brief Register a new collider
 *
 * A collider is used to detect and handle collisions of Lasers and Objects (Monster, Player or Wall).
 * 
 * @param c A pointer to the collider
 * @return uint8_t the id of the collider that can be used to remove it later
 */
uint8_t register_collider(collider* c) {
    ASSERT_LED(ERR_NULL_POINTER, (c != 0));

    if (count_colliders >= MAX_COLLIDERS) {
        for (uint8_t i = 0; i < count_colliders; ++i) {
            if (collider_deleted[i]) {
                colliders[i] = c;
                return i;
            }
        }

        return MAX_COLLIDERS;
    }

    colliders[count_colliders] = c;
    return count_colliders++;
}

/**
 * @brief Removes a given collider
 * 
 * @param index the id of the collider
 */
void remove_collider(uint8_t index) {
    ASSERT_LED(ERR_OVERFLOW, (index < MAX_COLLIDERS));
    collider_deleted[index] = true;
}

/**
 * @brief Returns if a laser ray colliders with a collider
 * 
 * @param l a pointer to the laser ray
 * @param colliderIndex gets set to the index of the collider it collided with
 * @return true if the laser collided with anything
 * @return false if the laser didn't collide with anything
 */
bool collides(laser *l, uint8_t *colliderIndex) {
    ASSERT_LED(ERR_NULL_POINTER, (l != 0));
    
    for (uint8_t i = 0; i < count_colliders; ++i) {
        if (collider_deleted[i]) continue;
        collider* c = colliders[i];

        bool insideX = l->x > c->x && l->x < c->x + c->width;
        bool insideY = l->y > c->y && l->y < c->y + c->height;

        *colliderIndex = i;
        if (insideX && insideY) return true;
    }

    return false;
}

/**
 * @brief Spawn a new laser ray
 * 
 * @param x the x position
 * @param y the y position
 * @param direction the direction it is supposed to move in
 */
void shoot_laser(uint8_t x, uint8_t y, int8_t direction) {
    laser l;
    l.x = x;
    l.y = y;
    l.dir = direction;
    l.deleted = false;

    if (count_lasers >= MAX_LASERS) {
        for (int i = 0; i < MAX_LASERS; ++i) {
            if (lasers[i].deleted) {
                lasers[i] = l;
                break;
            }
        }

        return;
    }

    lasers[count_lasers++] = l;
}

/**
 * @brief Animates all currently active laser rays
 *
 * Used to track the movement of laser rays.
 */
void animate_lasers(void) {
    for (int8_t i = 0; i < count_lasers; ++i) {
        laser* l = &lasers[i];
        if (l->deleted) continue;

        int8_t newY = (int8_t)l->y + 2 * l->dir;
        if (newY < 0 || newY > 64) l->deleted = true;
        else l->y += l->dir;

        uint8_t collider;
        if (collides(l, &collider)) {
            callback(collider);
            l->deleted = true;
        }
    }
}

/**
 * @brief Renders all currently active laser rays
 */
void render_lasers(void) {
    for (uint8_t i = 0; i < count_lasers; ++i) {
        laser* l = &lasers[i];
        if (l->deleted) continue;

        for (uint8_t y = 0; y < 4; ++y) {
            if ((int8_t)l->y - (int8_t)y < 0) continue;
            set_pixel(l->x, l->y - y, true);
        }
    }

#ifdef DEBUG_COLLISSION
    for (uint8_t i = 0; i < count_colliders; ++i) {
        collider* c = colliders[i];
        if (collider_deleted[i]) continue;
        fill_rect(c->x, c->y, c->width, c->height, true);
    }
#endif
}

/**
 * @brief Reset everything
 * 
 * This function is used to reset all variables in order to start a new game.
 */
void reset_combat(void){
    memset(&lasers[0], 0, sizeof(lasers));
    count_lasers = 0;
    memset(&colliders[0], 0, sizeof(colliders));
    count_colliders = 0;
    memset(collider_deleted, 0, sizeof(collider_deleted));
}