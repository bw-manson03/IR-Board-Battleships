/** @file   missile.c
    @author HanByeol Yang(hya62), Blake W. Manson(bma206) - Group 509
    @date   18 October 2023
    @brief  craete missile heap.
*/


#ifndef MISSILE_H
#define MISSILE_H

#include "object.h"

/**
 * @brief create missile object to shoot
 * 
 * @return pointer to memory for missile object
*/
object_t* missile_init(void);

#endif //MISSILE_H