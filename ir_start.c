/** @file   ir_start.h
    @author Blake W. Manson(bma206) - Group 509
    @date   18 October 2023
    @brief  handles IR transmit to transmit data to opponent.
*/

#include "ir_uart.h"
#include "ir_start.h"

/**
 * @brief send transmit data to other player
 * 
 * @param order 8 bits of data to transmit to other player
*/
void send(char order)
{
    ir_uart_putc(order);
}

/**
 * @brief receive the opponent transmitted value 
 * 
 * @return 8 bits transmitted value from others
*/
uint8_t receive(void)
{
    uint8_t done = 0xFF;
    if (ir_uart_read_ready_p()){
        done = ir_uart_getc();
    }

    return done;
}

/**
 * @brief check if the player has finished setup, waits for other player to finish, then sets turn order
 * 
 * @return the turn position for the player
*/
void ir_init(void)
{
    ir_uart_init();
}
