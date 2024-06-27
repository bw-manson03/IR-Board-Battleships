/** @file   game.c
    @author HanByeol Yang(hya62), Blake W. Manson(bma206) - Group 509
    @date   18 October 2023
    @brief  main file - program is start from this file
*/


#include "system.h"
#include "ir_start.h"
#include "object.h"
#include "control.h"
#include "progress.h"

/**
 * @brief main function to run program - Described sequence of the program.
*/
int main (void)
{
    system_init ();
    ir_init();
    led_interface_init();
    control_interface_init();
    bool finish_setup = false;
    uint8_t turn = 0;
    // initiallise veriable to 0xFF - use 0 for declare game need to be continue
    uint8_t game_over = 0xFF;
    
    while(1) {
    
        // game setup
        if(!finish_setup) {
            instruction_set(WELCOME);
            ship_init();
            finish_setup = true;
            instruction_set(CHOOSE_ORDER);
            turn = choose_order();
        }
        
        // player shoot missile
        if(turn == 1) {
            instruction_set(PLAYER_TURN);
            object_t* missile = missile_control();
            uint8_t coordinates = ((missile->col) << 4) | (missile->row);   // combine row and col data into 8 bits
            send(coordinates);
            free_missile(missile); // free heap
            turn++;

            // reset veriable to 0xFF - use 0 for declare game need to be continue
            game_over = 0xFF;
            // check opponent's defeat declaration
            while(game_over == 0xFF) {
                game_over = receive();
            }

        // player standby for opponent's missile impact
        } else if (turn == 2){
            instruction_set(OPPONENT_TURN);
            missile_impact(&receive);   
            turn--;

            // check if player loose all ship and if so, declare player's defeat to 
            if(check_game_over()) {
                // player is defeated
                game_over = 1;
            } else {
                // game continue
                game_over = 0;
            }
            send(game_over);
        }

        // game over
        if(game_over == 1) {
            if(check_game_over()) {
                instruction_set(LOSE);
            } else {
                instruction_set(WIN);
            }

            finish_setup = false;
            turn = 0;
        }

    }
}
