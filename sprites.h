/**
 * @file sprites.h
 * @brief Sprites for the game
 */

#ifndef __SPRITES_H
#define __SPRITES_H

/**
 * @brief The maximum allowed height of a sprite
 */
#define MAX_SPRITE_HEIGHT 16

/**
 * @brief Representing a Sprite which can be drawn onto the display
 * 
 * @param data An Array which contains uint16_t variables, matching the pixels of a row
 * @param width The width of a sprite in pixels
 * @param height The height of a sprite in pixels
 */
typedef struct Sprite {
  uint16_t data[MAX_SPRITE_HEIGHT];
  uint8_t width;
  uint8_t height;
} sprite;

extern const sprite MONSTER;
extern const sprite MONSTER2;
extern const sprite PLAYER;
extern const sprite NUMBERS[];
extern const sprite WALL;
extern const sprite WALL_DAMAGE_1;
extern const sprite WALL_DAMAGE_2;
extern const sprite WALL_DAMAGE_3;
extern const sprite HEART;
extern const sprite LETTER_G;
extern const sprite LETTER_A;
extern const sprite LETTER_M;
extern const sprite LETTER_E;
extern const sprite LETTER_O;
extern const sprite LETTER_V;
extern const sprite LETTER_R;

/**
 * @brief draw the given sprite at the given position
 * 
 * @param x the x position
 * @param y the y position
 * @param figure a pointer to the sprite
 */
void draw_sprite(uint8_t x, uint8_t y, const sprite *figure);

#endif