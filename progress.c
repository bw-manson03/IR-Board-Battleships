/** @file   progress.c
    @author HanByeol Yang(hya62), Blake W. Manson(bma206) - Group 509
    @date   18 October 2023
    @brief  handles LED1 and led-Matrix
*/
#include <stdlib.h>

#include "missile.h"
#include "control.h"
#include "system.h"
#include "led.h"
#include "pacer.h"
#include "display.h"
#include "tinygl.h"
#include "../fonts/font5x5_1.h"
#include "progress.h"

#define PACER_RATE 500
#define MESSAGE_RATE 25


/**
 * @brief free missile object from memory
 * 
 * @param missile missile to be removed
*/
void free_missile(object_t* missile) 
{
    free(missile);
}

/**
 * @brief position the missile
 * 
 * @return pointer to missile after being controlled
*/
object_t* missile_control(void) 
{
    object_t* missile = missile_init();
    draw_object(missile, 1);
    object_control(missile);

    display_clear();
    display_update();

    return missile;
}


/**
 * @brief Show player's ship position, standing by until opponent missile information is sent to player
 * and check one of player's ship is hit.
 * 
 * @param receive Address value of receive function at ir_start module.
 * @return true if one of player ship is hit by opponent's missile,
 * @return false if none of player's ships is been hit.
 */
bool missile_impact(uint8_t (*receive)(void)) 
{
    
    /* 'get' variable will assign data from opponent that contains 
       combination of opponent's missle row and column data into 8bits.
       It is possible that result of row and column can be both 0.
       Thus we cannot initialise 'get' variable with 0 */

    uint8_t get = 0xFF; // there are no rows larger than or equal to 7 and no columns lager than or equal to 5
    uint8_t col = 0;
    uint8_t row = 0;
    show_ships();

    while(get == 0xFF) {
        pacer_wait();
        display_update();
        get = receive(); // try to receive opponent missile row/column
    }

    row = get & 0xF;
    col = get >> 4;
    display_clear();
    display_update();

    return check_ship_hit(col, row);
}

/**
 * @brief connect check_all_ship_destroyed function at control.c
 * 
 * @return true if check_all_ship_destroyed function returns true or 
 * @return false if check_all_ship_destroyed function returns false
 */
bool check_game_over(void) {

    return check_all_ship_destroyed();
}

/**
 * @brief execute row movement of object
 * 
 * @param prev Starting point of the previous row of objects
 * @param curr Starting point of current row of object
 * @param col column location of object
 */
void move_in_row(uint8_t prev, uint8_t curr, uint8_t col)
{
    display_pixel_set(col, prev, 0);
    display_pixel_set(col, curr, 1);
}

/**
 * @brief check pixel in led-matrix is on
 * 
 * @param col matrix column
 * @param row matrix row
 * @return true if pixel is on,
 * @return false if pixel is off
 */
bool collision_check(uint8_t col, uint8_t row)
{
    return display_pixel_get(col, row);
}

/**
 * @brief show object on led-matrix to control/move object using navswitch. If object parameter is 'NULL',
 * then there is no object need control thus it will just show display.
 * 
 * @param object the object being moved
 */
void object_control(object_t* object)
{
    bool done = 0;
    while(!done){

        pacer_wait();
        display_update();
        done = positioning(object);
    }
}

/**
 * @brief draw or remove ship into led-matrix
 * 
 * @param object structure of object that has start row, col and size information
 * @param val led status value 1 for led on 0 for other
*/
void draw_object(object_t* object, bool val)
{
    for(uint8_t i = object->row; i < (object->row + object->size+1); i++) {
        display_pixel_set(object->col, i, val);
    }
}

/**
 * @brief initilaise a start point of the ship to set its position
 * 
 * @param ship ship object informations to be initialise
 */
void ship_initialise_start_point(object_t* ship) 
{
    while(1) {
        if((ship->row + ship->size >= LEDMAT_ROWS_NUM))
        {
            ship->col++;
            ship->row = 0;
        } else if(display_pixel_get(ship->col, ship->row + ship->size) || display_pixel_get(ship->col, ship->row)) {
            ship->row++;
        } else {
            break;
        }
    }    
}

/**
 * @brief Initial game setup
*/
void ship_init(void)
{
    
    // indicate player can control their funkits
    led1_on();

    for(uint8_t i=0; i < TOTAL_NUM_SHIP; i++){     
        object_t ship = {.size = SHIP[i], .col=2, .row=1};

        // need to prevent ship's collision at the beginning of initialisation
        ship_initialise_start_point(&ship);
        draw_object(&ship, 1);
        object_control(&ship);
        //save ship information
        store_ship(i, ship);
    }
    
    // clear display to hide your ship position to opponent
    display_clear();
    display_update();

    // indicate players cannot control their funkits
    led1_off();
}


/**
 * @brief choose the order of play for the players
 * 
 * @return position, either 1 or 2
*/
uint8_t choose_order(void)
{
    uint8_t position = 1;
    char* position_text = "1\0";

    uint8_t curr_position = 1;
    while(curr_position != 0) {
        pacer_wait();
        if(curr_position == 1) {
            position_text = "1\0";
        } else if (curr_position == 2) {
               position_text = "2\0";
        }
        tinygl_text(position_text);
        tinygl_update();
        position = curr_position;
        curr_position = order_positioning(curr_position);

    }

    tinygl_clear();
    return position;
}


/**
 * @brief trigger function to setup to show message on led-matrix
 * 
 * @param message the message type to be sent
*/
void instruction_set(message_t message)
{

    bool stop = false;
  
    switch (message)
    {
        case WELCOME:
            tinygl_text("Push any button to start!");
            while(!stop) {
                pacer_wait();
                tinygl_update();
                stop = any_push();
            }
            display_clear();
            display_update();

            break;
        case PLAYER_TURN:
            led1_on();
            tinygl_text("Fire your missile");
            stop = false;
            while(!stop) {
                pacer_wait();
                tinygl_update();
                stop = any_push();
            }
            display_clear();
            display_update();

            break;

        case OPPONENT_TURN:
            led1_off();
            tinygl_text("Prepare for impact");
            stop = false;
            while(!stop) {
                pacer_wait();
                tinygl_update();
                stop = any_push();
            }
            display_clear();
            display_update();
            break;

        case WIN:
            tinygl_text("You win\0");

            //loop will execute around 5 seconds
            while (!stop) {
                pacer_wait();
                tinygl_update();
                stop = any_push();
            }
            display_clear();
            display_update();
            break;
       
        case LOSE:
            led1_off();
            tinygl_text("You Lose!\0");

            while (!stop) {
                pacer_wait();
                tinygl_update();
                stop = any_push();
            }

            display_clear();
            display_update();
            break;

        case CHOOSE_ORDER:
            led1_off();
            tinygl_text("Choose order\0");

            while (!stop) {
                pacer_wait();
                tinygl_update();
                stop = any_push();
            }

            display_clear();
            display_update();
            break;

    }

}


/**
 * @brief turn off led1(BLUE LED)
*/
void led1_off(void)
{
    led_set(LED1, 0);
}

/**
 * @brief turn on led1(BLUE LED)
*/
void led1_on(void)
{
    led_set(LED1, 1);
}

/**
 * @brief initialise LED1(BLUE LED) and LED-matrix
*/
void led_interface_init(void)
{
    pacer_init(PACER_RATE);
    led_init();
    tinygl_init(PACER_RATE);
    tinygl_font_set (&font5x5_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
    tinygl_text_dir_set (TINYGL_TEXT_DIR_ROTATE);
}