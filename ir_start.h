/** @file   ir_start.h
    @author Blake W. Manson(bma206) - Group 509
    @date   18 October 2023
    @brief  handles IR transmit to transmit data to opponent.
*/

#ifndef IR_START_H
#define IR_START_H

#include <stdint.h>

/**
 * @brief send transmit data to other player
 * 
 * @param order 8 bits of data to transmit to other player
*/
void send(char order);

/**
 * @brief receive the opponent transmitted value 
 * 
 * @return 8 bits transmitted value from others
*/
uint8_t receive(void);

/**
 * @brief check if the player has finished setup, waits for other player to finish, then sets turn order
 * 
 * @return the turn position for the player
*/
void ir_init(void);

#endif //IR_START_H