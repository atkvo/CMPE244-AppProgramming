/*
 * i2c_master_task.cpp
 *
 *  Created on: Apr 16, 2017
 *      Author: James
 */

#include "i2c_master_task.hpp"
#include "io.hpp"
#include <stdio.h>
#include "utilities.h"

i2c_master_task::i2c_master_task(uint8_t priority) : scheduler_task("i2cm",2048,priority) {
    setRunDuration(33);
}


int16_t map(int16_t x, int16_t in_min, int16_t in_max, int16_t out_min, int16_t out_max)
{
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


bool i2c_master_task::init(void) {
    return true;
}

bool i2c_master_task::run(void *p) {
    static int16_t x=0;
    static int16_t y=0;
    static int16_t prev_x = 0;
    static int16_t prev_y = 0;
    prev_x = x;
    prev_y = y;

//    vTaskDelay(30);
    // value is between 0 and 2048
//    x = map(AS.getX(), -2047, 2047, 0, 239);
//    y = map(AS.getY(), -2047,2047,0,319);
    if (SW.getSwitch(1)) {
        y++;
    }
    if (SW.getSwitch(2)) {
        y--;
    }
    if (SW.getSwitch(3)) {
        x--;
    }
    if (SW.getSwitch(4)) {
        x++;
    }


    if (prev_x != x) {
        printf("x: %4d y: %4d\n",x,y);
        if(I2C2::getInstance().writeReg(0xaf,0x01,(uint8_t)x)) {
            printf("X written ");
        } else {
            printf("X not written ");
        }
    }
    if (prev_y != y) {
        if(I2C2::getInstance().writeReg(0xaf,0x02,(uint8_t)y)) {
            printf("Y written\n");
        } else {
            printf("Y not written\n");
        }
    }

    return true;
}

