/** @file   control.h
    @author HanByeol Yang(hya62), Blake W. Manson(bma206) - Group 509
    @date   18 October 2023
    @brief  handles navswitch, manages data processing related to game progress.
*/

#ifndef BATTLESHIP_H
#define BATTLESHIP_H

#include <stdint.h>
#include <stdbool.h>

#include "object.h"

/**
 * @brief check if all ships have been destroyed
 * 
 * @return true if they have,
 * @return false otherwise
*/
bool check_all_ship_destroyed(void);

/**
 * @brief Store ship info into static array ships
*/
void store_ship(uint8_t index, object_t ship);

/**
 * @brief Display all ships onto the LED matrix
*/
void show_ships(void);

/**
 * @brief Check if a ship was hit by a missile. If yes, remove the ship from the matrix.
 * 
 * @param missile_col the column location of the missile fired
 * @param missile_row the row location of the missile fired
 * @return true if a ship is hit,
 * @return false otherwise
*/
bool check_ship_hit(uint8_t missile_col, uint8_t missile_row);

/**
 * @brief change order of positions based on naswitch input
 * 
 * @param curr_position the current position of the player
 * @return the new position of the player
*/
uint8_t order_positioning(uint8_t curr_position);

/**
 * @brief ship shifting to initialise ships' position. Ship will not move if ship is going out of bound of led-matirx or
 * other ship is blocking its way.
 * 
 * @param ship structure of ship that has row, col, and its size
 * @return true when NAVSWITCH_PUSH push event is happen - represent ship position init is finish,
 * @return false otherwise
*/
bool positioning(object_t* object);

/**
 * @brief Check if any navswitch buttons is pushed
 * 
 * @return true if has been pushed,
 * @return false otherwise
*/
bool any_push(void);

/**
 * @brief initialise navswitch and button
*/
void control_interface_init(void);

#endif //BATTLESHIP_H