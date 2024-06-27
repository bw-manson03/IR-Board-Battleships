/** @file   progress.h
    @author HanByeol Yang(hya62), Blake W. Manson(bma206) - Group 509
    @date   18 October 2023
    @brief  handles LED1 and led-Matrix
*/
#ifndef PROGRESS_H
#define PROGRESS_H

#include <stdint.h>
#include <stdbool.h>

#include "object.h"


typedef enum {
    WELCOME,
    PLAYER_TURN,
    OPPONENT_TURN,
    WIN,
    LOSE,
    CHOOSE_ORDER
} message_t;

/**
 * @brief choose the order of play for the players
 * 
 * @return position, either 1 or 2
*/
uint8_t choose_order(void);

/**
 * @brief free missile object from memory
 * 
 * @param missile missile to be removed
*/
void free_missile(object_t* missile);

/**
 * @brief position the missile
 * 
 * @return pointer to missile after being controlled
*/
object_t* missile_control(void);

/**
 * @brief Show player's ship position, standing by until opponent missile information is sent to player
 * and check one of player's ship is hit.
 * 
 * @param receive Address value of receive function at ir_start module.
 * @return true if one of player ship is hit by opponent's missile or
 * @return false if none of player's ships is been hit.
 */
bool missile_impact(uint8_t (*receive)(void));

/**
 * @brief connect check_all_ship_destroyed function at control.c
 * 
 * @return true if check_all_ship_destroyed function returns true or 
 * @return false if check_all_ship_destroyed function returns false
 */
bool check_game_over(void);

/**
 * @brief execute row movement of object
 * 
 * @param prev Starting point of the previous row of objects
 * @param curr Starting point of current row of object
 * @param col column location of object
 */
void move_in_row(uint8_t prev, uint8_t curr, uint8_t col);

/**
 * @brief check pixel in led-metrix is on
 * 
 * @param col matrix column
 * @param row matrix row
 * @return true if pixel is on,
 * @return false if pixel is off
 */
bool collision_check(uint8_t col, uint8_t row);

/**
 * @brief show object on led-matrix to control/move object using navswitch
 * 
 */
void object_control(object_t* object);

/**
 * @brief draw or remove ship into led-matriex
 * @param object structure of object that has start row, col and size information
 * @param val led status value 1 for led on 0 for other
*/
void draw_object(object_t* object, bool val);

/*Initial game setup*/
void ship_init(void);

/**
 * @brief turn on led1(BLUE LED)
*/
void led1_on(void);

/**
 * @brief turn off led1(BLUE LED)
*/
void led1_off(void);


/**
 * @brief trigger function to setup to show message on led-matrix
*/
void instruction_set(message_t message);

/**
 * @brief initialise LED1 and LED-matrix
*/
void led_interface_init(void);


#endif /*PROGRESS_H*/
