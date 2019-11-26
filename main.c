/**
 * @file main.c
 * @brief Main 
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <util/delay_basic.h>

#include "wait.h"
#include "input.h"
#include "rendering.h"
#include "sprites.h"
#include "combat.h"

#define SHOOT_TIMEOUT 500
#define MONSTER_COUNT 10
#define WALL_COUNT 3
#define MAX_WALL_LIFES 3
#define DELAY_TIME 20
#define BUTTON_CLICKED 1023
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_WIDTH_CENTER 64
#define PAUSE_DURATION 800

#define DEFAULT_BOUND_LEFT 0
#define DEFAULT_BOUND_RIGHT 35


/************* MONSTERS *************/
collider default_monsters[MONSTER_COUNT] = {
    {0, 10, 13, 9},
    {20, 10, 13, 9},
    {40, 10, 13, 9},
    {60, 10, 13, 9},
    {80, 10, 13, 9},
    {0, 25, 13, 9},
    {20, 25, 13, 9},
    {40, 25, 13, 9},
    {60, 25, 13, 9},
    {80, 25, 13, 9}
};
collider monsters[MONSTER_COUNT];
bool dead_monsters[MONSTER_COUNT];
uint8_t count_dead_monsters = 0;
uint8_t monster_colliders[MONSTER_COUNT];
uint8_t monster_speed = 3;
int8_t monsters_left_bound = DEFAULT_BOUND_LEFT;
int8_t monsters_right_bound = DEFAULT_BOUND_RIGHT;
uint8_t monsters_movement_sleep = 0;
uint8_t monsters_movement_direction = 1;

/************** WALLS ***************/
collider default_walls[WALL_COUNT] = {
    {20, 45, 13, 5},
    {64 - 6, 45, 13, 5},
    {94, 45, 13, 5}
};
collider walls[WALL_COUNT];
uint8_t wall_colliders[WALL_COUNT];
uint8_t wall_state[WALL_COUNT] = {0,0,0};

/*************** GAME ***************/
int score = 0;
uint64_t gameTime = 0;
uint64_t gameTimePrev = 0;
uint64_t pauseUntil = 0;

/************* CONTROLS *************/
bool pressed = false;
uint64_t shootTimeout = 0;

/************** PLAYER **************/
collider default_player = {SCREEN_WIDTH_CENTER - 6, SCREEN_HEIGHT - 11, 13, 11};
collider player;
uint8_t playerCollider;
bool playerFlashing = false;
uint16_t playerFlashCount = 0;
uint8_t lives = 3;



/**
 * @brief The Callback function which is executed if an object is hit by a laser ray.
 * 
 * This functions handles collisions between objects and laser rays. If the figure of the player is hit, the game is paused and one life is removed.
 * If a monster is hit, it gets deleted. Depending on the amount of monsters, the speed is increased. If a wall is hit, the state which represents the 
 * amount of destruction is increased and the wall is deleted if necessary.
 * 
 * @param index The index of the collider which has been hit.
 */
void onCollide(uint8_t index) {
    if (index == playerCollider) {
        pauseUntil = gameTime + PAUSE_DURATION;
        playerFlashing = true;
    } else {
        for (uint8_t i = 0; i < MONSTER_COUNT; ++i) {
            if (monster_colliders[i] == index) {
                remove_collider(monster_colliders[i]);
                dead_monsters[i] = true;
                count_dead_monsters++;
                score += 100;

                if (count_dead_monsters > 6) monster_speed = 1;
                else if (count_dead_monsters > 3) monster_speed = 2;

                return;
            }
        }
        
        for(int8_t i = 0; i < WALL_COUNT; ++i){
            if(wall_colliders[i] == index){
                if(wall_state[i] == MAX_WALL_LIFES)
                {
                    remove_collider(wall_colliders[i]);
                }
                
                wall_state[i]++;
            

            }

        }

    }
}

/**
 * @brief Reset all variables for a clean start of the game
 * 
 */
void init_game(void) {
    /************* MONSTERS *************/
    memcpy(&monsters[0], &default_monsters[0], sizeof(default_monsters));
    memset(&dead_monsters[0], 0, sizeof(dead_monsters));
    count_dead_monsters = 0;
    memset(&monster_colliders[0], 0, sizeof(monster_colliders));
    monster_speed = 3;
    monsters_left_bound = DEFAULT_BOUND_LEFT;
    monsters_right_bound = DEFAULT_BOUND_RIGHT;
    monsters_movement_sleep = 0;
    monsters_movement_direction = 1;

    /************** WALLS ***************/
    memcpy(&walls[0], &default_walls[0], sizeof(default_walls));
    memset(&wall_colliders[0], 0, sizeof(wall_colliders));
    memset(&wall_state[0], 0, sizeof(wall_state));

    /*************** GAME ***************/
    score = 0;
    gameTime = 0;
    gameTimePrev = 0;
    pauseUntil = 0;

    /************* CONTROLS *************/
    pressed = false;
    shootTimeout = 0;

    /************** PLAYER **************/
    memcpy(&player, &default_player, sizeof(default_player));
    playerCollider = 0;
    playerFlashing = false;
    playerFlashCount = 0;
    lives = 3;

    reset_combat();

    register_collide_callback(onCollide);
    playerCollider = register_collider(&player);

    for (int i = 0; i < MONSTER_COUNT; ++i) {
        monster_colliders[i] = register_collider(&monsters[i]);
    }

    for (int i = 0; i < WALL_COUNT; ++i) {
        uint8_t collId = register_collider(&walls[i]);
        wall_colliders[i] = collId;
    }
}

/**
 * @brief Draws the score on the screen
 * 
 * @param x X-Coordinate on the screen.
 * @param y Y-Coordinate on the screen.
 */
void drawScore(uint8_t x, uint8_t y) {
    char scoreStr[12];
    sprintf(scoreStr, "%d", score);

    int numX = x;
    for (uint8_t i = 0; i < strlen(scoreStr); ++i) {
        char c = scoreStr[i];
        uint8_t num = c - 48;
        const sprite* numSprite = &NUMBERS[num];

        draw_sprite(numX, y, numSprite);
        numX += numSprite->width + 1;
    }
}

/**
 * @brief Draws the remaining lifes of the player as heart symbols on the screen
 * 
 */
void drawLifes(void) {
    uint8_t x = 128 - 11;
    for (uint8_t i = lives; i > 0; --i) {
        draw_sprite(x - 12 * (lives - i), 0, &HEART);
    }
}

/**
 * @brief Handles the movement of the player
 * 
 * First, the x value of the Joystick is retrieved. This value is translated into a left/right movement or a shot.
 * This function also ensures that the player cannot move out of the display
 */
void player_movement(void) {
    int16_t x, y;
    get_input(&x, &y);

    if (x == BUTTON_CLICKED && !pressed) { // pressed
        pressed = true;

        if (gameTime >= shootTimeout) {
            shoot_laser(player.x + 6, 64 - 11, DIRECTION_UP);
            shootTimeout = gameTime + SHOOT_TIMEOUT;
        }
    } else if (x != BUTTON_CLICKED) {
        pressed = false;
    }

    if (x == BUTTON_CLICKED) x = 512; // set the value to something within the deadzone

    if (x > 550 && player.x < 115) player.x++;
    else if (x < 480 && player.x > 0) player.x--;
}

/**
 * @brief Calculates how far the monsters can move, depending on which of them are alive.
 * 
 */
void calculate_bounds(void) {
    int8_t newLeftBound = DEFAULT_BOUND_LEFT;
    for (uint8_t i = 0; i < 5; ++i) {
        if (dead_monsters[i] && dead_monsters[i + 5]) newLeftBound -= 20;
        else break;
    }
    monsters_left_bound = newLeftBound;

    int8_t newRightBound = DEFAULT_BOUND_RIGHT;
    for (int8_t i = 4; i >= 0; --i) {
        if (dead_monsters[i] && dead_monsters[i + 5]) newRightBound += 20;
        else break;
    }
    monsters_right_bound = newRightBound;
}

/**
 * @brief Generates random shots by alive monsters.
 * 
 */
void shoot_monsters(void) {
    for (uint8_t i = 0; i < MONSTER_COUNT; ++i) {
        // if this monster is in the upper row and the one below it is alive, don't shoot!
        if (i < 5 && !dead_monsters[i + 5]) continue;
        if (dead_monsters[i]) continue;

        if (rand() % 100 < 2) { // 2 percent chance of shooting each frame
            shoot_laser(monsters[i].x + 6, monsters[i].y + monsters[i].height, DIRECTION_DOWN);
        }
    }
}

/**
 * @brief Makes the player blink
 * 
 */
void animatePlayer(void) {
    if (playerFlashing) {
        playerFlashCount++;
        if (playerFlashCount >= 6) playerFlashCount = 0;
    }
    if (!playerFlashing || (playerFlashing && playerFlashCount < 3)) {
        draw_sprite(player.x, 53, &PLAYER);
    }
}

/**
 * @brief Renders the game.
 *
 * This function handles the game rendering and calls all necessary functions for the gameplay. 
 */
void renderGame(void) {
    if (gameTime >= pauseUntil) {
        if (playerFlashing) {
            playerFlashing = false;
            lives--;
        }

        player_movement();
        shoot_monsters();

        if(monsters_movement_sleep == 0) {
            calculate_bounds();
            for (int i = 0; i < MONSTER_COUNT; ++i) monsters[i].x += monsters_movement_direction;
            if(monsters[0].x == monsters_left_bound || monsters[0].x == monsters_right_bound) {
                monsters_movement_direction = -monsters_movement_direction;
            }
        }

        monsters_movement_sleep++;
        monsters_movement_sleep = monsters_movement_sleep % monster_speed;

        animate_lasers();
    }

    drawScore(0, 0);
    drawLifes();
    animatePlayer();

    for(uint8_t i = 0; i < MONSTER_COUNT; ++i) {
        if (dead_monsters[i]) continue;
        if(i <= 4)
            draw_sprite(monsters[i].x, monsters[i].y, &MONSTER);
        else
            draw_sprite(monsters[i].x, monsters[i].y, &MONSTER2);
    }


    for(uint8_t i = 0; i < WALL_COUNT; i++) {
        switch(wall_state[i]){
            case 0:
                draw_sprite(walls[i].x, walls[i].y, &WALL);
                break;
            case 1:
                draw_sprite(walls[i].x, walls[i].y, &WALL_DAMAGE_1);
                break;
            case 2:
                draw_sprite(walls[i].x, walls[i].y, &WALL_DAMAGE_2);
                break;
            case 3:
                draw_sprite(walls[i].x, walls[i].y, &WALL_DAMAGE_3);
                break;
        }

    }
    
    render_lasers();

    flash_buffer();
    clear_buffer();

    delay_millis(DELAY_TIME);
    gameTimePrev = gameTime;
    gameTime += DELAY_TIME;
}

/**
 * @brief Renders the "Game Over" screen.
 * 
 * Writes "Game Over" and the reached score on the screen. If the Joystick is clicked, the Game gets restarted.
 */
void renderGameOver(void) {
    uint8_t len = 8, x = 20;
    sprite GAME_OVER[] = {
        LETTER_G,
        LETTER_A,
        LETTER_M,
        LETTER_E,
        LETTER_O,
        LETTER_V,
        LETTER_E,
        LETTER_R
    };

    for (uint8_t i = 0; i < len; ++i) {
        sprite* letter = &GAME_OVER[i];
        draw_sprite(x, 14, letter);
        x += letter->width + 2;

        if (i == 3) x += 10;
    }

    uint16_t div = 10;
    uint8_t numScoreLetters = 1;
    while (true)  {
        if (score / div < 1) break;
        div *= 10;
        numScoreLetters++;
    }

    drawScore(68 - numScoreLetters * 4, 32);
    
    flash_buffer();
    clear_buffer();

    while (true){
        int16_t x, y;
        get_input(&x, &y);
        if (x == BUTTON_CLICKED) {
            init_game();
            pressed = true;
            break;
        }
    }
}


int main(void) {
    init_input();
    init_buffer();
    time_t t;
    srand((unsigned) time(&t));

    init_game();

    while (true) {
        if (lives > 0 && !(count_dead_monsters == MONSTER_COUNT)) renderGame();
        else renderGameOver();
    }
}