/** @file   object.h
    @author HanByeol Yang(hya62) - Group 509
    @date   18 October 2023
    @brief  game object definitions are here.
*/

#ifndef OBJECT_H
#define OBJECT_H

#include <stdint.h>
#include <stdbool.h>

// define ship and missile size 
#define LARGE 3
#define MEDIUM 2
#define SMALL 1
#define MISSILE 0

#define TOTAL_NUM_SHIP 3

// define array SHIP and to use for 'for loop' at ship_init function
#define SHIP (uint8_t[]){LARGE, MEDIUM, SMALL}

/**
 * Define object start row, start coloumn and its size
*/
typedef struct {
    uint8_t size;
    uint8_t col;
    uint8_t row;
    bool destroy;
} object_t;

#endif // OBJECT_H