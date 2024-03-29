#include "rendering.h"
#include "sprites.h"
#include "sprites_numbers.h"

/**
 * @brief The sprite of a monster.
 * 
 */
const sprite MONSTER = {
  {
    0b0001000001000000,
    0b0000100010000000,
    0b0011111111100000,
    0b0110111110110000,
    0b1111111111111000,
    0b1011111111101000,
    0b1011111111101000,
    0b0001000001000000,
    0b0000110110000000
  },
  13,
  9
};

/**
 * @brief The sprite of a monster.
 * 
 */
const sprite MONSTER2 = {
  {
    0b0000011100000000,
    0b0000111110000000,
    0b0011111111100000,
    0b0111111111110000,
    0b0110011100110000,
    0b0111111111110000,
    0b0001011101000000,
    0b0011000001100000,
    0b1100000000011000
  },
  13,
  9
};

const sprite NUMBERS[] = {
  NUMBER0, NUMBER1, NUMBER2, NUMBER3,
  NUMBER4, NUMBER5, NUMBER6, NUMBER7,
  NUMBER8, NUMBER9
};

/**
 * @brief The sprite of the players figure.
 * 
 */
const sprite PLAYER = {
  {
    0b0000001000000000,
    0b0000001000000000,
    0b0000001000000000,
    0b0000001000000000,
    0b1100001000011000,
    0b0011111111100000,
    0b0000001000000000,
    0b0000010100000000,
    0b0011110111100000,
    0b0011111111100000,
    0b1111111111111000,
    0b0011000001100000
  },
  13,
  11
};

/**
 * @brief The sprite of a wall without any damages.
 * 
 */
const sprite WALL = {
  {
    0b0000111110000000,
    0b0001111111000000,
    0b0011111111100000,
    0b0111111111110000,
    0b1111100011111000
  },
  13,
  5
};

/**
 * @brief The sprite of a wall with low damages.
 * 
 */
const sprite WALL_DAMAGE_1 = {
  {
    0b0000110110000000,
    0b0001111001000000,
    0b0011111111100000,
    0b0111111111110000,
    0b1111100011111000
  },
  13,
  5
};

/**
 * @brief The sprite of a wall with a higher amount of damages.
 * 
 */
const sprite WALL_DAMAGE_2 = {
  {
    0b0000010110000000,
    0b0001011001000000,
    0b0011001111100000,
    0b0111111111110000,
    0b1111100011111000
  },
  13,
  5
};

 	
/**
 * @brief The sprite of a wall with the highest amount of possible damages.
 * 
 */
const sprite WALL_DAMAGE_3 = {

  {
   0b0000010000000000,
   0b0001010000000000,
    0b0001000100100000,
    0b0101111100110000,
    0b1110100011111000
  },
  13,
  5
};



/**
 * @brief The sprite of a heart.
 * 
 */
const sprite HEART = {
  {
    0b0011000110000000,
    0b0111101111000000,
    0b1111111111100000,
    0b1111111111100000,
    0b0111111111000000,
    0b0011111110000000,
    0b0001111100000000,
    0b0000111000000000,
    0b0000010000000000,
  },
  11,
  9
};

/**
 * @brief The sprite for the letter G.
 * 
 */
const sprite LETTER_G = {
    {
      0b0011111000000000,
      0b0111111100000000,
      0b1110001100000000,
      0b1100000000000000,
      0b1100000000000000,
      0b1100011100000000,
      0b1100011100000000,
      0b1100001100000000,
      0b1100001100000000,
      0b1110001100000000,
      0b0111111100000000,
      0b0011111000000000,
    },
    8,
    12
};

/**
 * @brief The sprite for the letter A.
 * 
 */
const sprite LETTER_A = {
    {
      0b0011110000000000,
      0b0111111000000000,
      0b0111111000000000,
      0b1110011100000000,
      0b1110011100000000,
      0b1110011100000000,
      0b1110011100000000,
      0b1111111100000000,
      0b1111111100000000,
      0b1110011100000000,
      0b1110011100000000,
      0b1110011100000000,
    },
    8,
    12
};

/**
 * @brief The sprite for the letter M.
 * 
 */
const sprite LETTER_M = {
    {
      0b1100000011000000,
      0b1100000011000000,
      0b1110000111000000,
      0b1110000111000000,
      0b1111001111000000,
      0b1111001111000000,
      0b1111001111000000,
      0b1101111011000000,
      0b1101111011000000,
      0b1101111011000000,
      0b1100110011000000,
      0b1100000011000000,
    },
    10,
    12
};

/**
 * @brief The sprite for the letter E.
 * 
 */
const sprite LETTER_E = {
    {
      0b1111111000000000,
      0b1111111000000000,
      0b1100000000000000,
      0b1100000000000000,
      0b1100000000000000,
      0b1111110000000000,
      0b1111110000000000,
      0b1100000000000000,
      0b1100000000000000,
      0b1100000000000000,
      0b1111111000000000,
      0b1111111000000000,
    },
    7,
    12
};

/**
 * @brief The sprite for the letter O.
 * 
 */
const sprite LETTER_O = {
    {
      0b0011110000000000,
      0b0111111000000000,
      0b1111111100000000,
      0b1100001100000000,
      0b1100001100000000,
      0b1100001100000000,
      0b1100001100000000,
      0b1100001100000000,
      0b1100001100000000,
      0b1111111100000000,
      0b0111111000000000,
      0b0011110000000000,
    },
    8,
    12
};

/**
 * @brief The sprite for the letter V.
 * 
 */
const sprite LETTER_V = {
    {
      0b1100000011000000,
      0b1100000011000000,
      0b1110000111000000,
      0b0110000110000000,
      0b0111001110000000,
      0b0111001110000000,
      0b0011001100000000,
      0b0011001100000000,
      0b0011001100000000,
      0b0001111000000000,
      0b0001111000000000,
      0b0000110000000000,
    },
    10,
    12
};

/**
 * @brief The sprite for the letter R.
 * 
 */
const sprite LETTER_R = {
    {
      0b1111110000000000,
      0b1111111000000000,
      0b1100011100000000,
      0b1100001100000000,
      0b1100011100000000,
      0b1111111000000000,
      0b1111110000000000,
      0b1100110000000000,
      0b1100111000000000,
      0b1100011000000000,
      0b1100011100000000,
      0b1100001100000000,
    },
    8,
    12
};

/**
 * @brief Draws a sprite on the screen
 * 
 * @param x Coordinate
 * @param y Coordinate
 * @param figure The figure that should be drawn
 */
void draw_sprite(uint8_t x, uint8_t y, const sprite *figure){
  for(uint8_t j = 0; j < figure->height; ++j) {
    for(uint8_t i = 0; i < figure->width; ++i) {
      set_pixel(x + i, y + j, (figure->data[j] >> (15 - i)) & 1);
    }
  }
}