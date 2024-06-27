/** @file   missile.c
    @author HanByeol Yang(hya62), Blake W. Manson(bma206) - Group 509
    @date   18 October 2023
    @brief  craete missile heap.
*/


#include <stdlib.h>
#include <stdbool.h>

#include "missile.h"

/**
 * @brief create missile object to shoot
 * 
 * @return pointer to memory for missile object
*/
object_t* missile_init(void)
{
    object_t* missile = malloc(sizeof(object_t));
    missile->row = 3;
    missile->col = 2;
    missile->size = MISSILE;
    missile->destroy=false;
    
    return missile;
}