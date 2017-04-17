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
#include "printf_lib.h"
#include "soft_timer.hpp"

enum Mode i2c_master_task::_mode;

i2c_master_task::i2c_master_task(uint8_t priority) : scheduler_task("i2cm",2048,priority),Mtimer(500) {
    setRunDuration(60);
    _mode = button;
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
    static int16_t real_x = 0;
    static int16_t real_y = 0;
    static int16_t prev_x = 0;
    static int16_t prev_y = 0;
    prev_x = x;
    prev_y = y;

    if (_mode == accel) {
        // set SJone slave to monitor mode
        I2C2::getInstance().writeReg(0xaf,0x06,0xAA);
        u0_dbg_printf("Set Monitor Mode\n");
        delay_ms(60);

        real_x = AS.getX();
        delay_ms(20);
        real_y = AS.getY();
        u0_dbg_printf("x: %3x y: %3x rx: %4d ry: %4d\n",x,y,real_x,real_y);

        Mtimer.reset();
        // set SJone slave to normal mode
        while(I2C2::getInstance().readReg(0xaf,0x06) != 0xBB) {

            if (Mtimer.expired()) {
                u0_dbg_printf("timeout\n");
                break;
            }
        }

        u0_dbg_printf("Monitor Mode Deactivated\n");

        // turn off I2C 2 interface
    //    LPC_I2C2->I2CONCLR = (1 << 6); // clear I2EN for I2C2
    //    vTaskDelay(30);
        // value is between 0 and 2048
    //    x = map(real_x, -2047, 2047, 0, 239);
    //    y = map(real_y, -2047,2047,0,319);
        x = map(real_x, -1024, 1024, 0, 239);
        y = map(real_y, -1024,1024,0,319);
        // turn on I2C 2 interface
    //    LPC_I2C2->I2CONSET = 0x44; // set I2EN for I2C2
    } else if (_mode == button) {
        if (SW.getSwitch(1)) {
            y+=2;
        }
        if (SW.getSwitch(2)) {
            y-=2;
        }
        if (SW.getSwitch(3)) {
            x-=2;
        }
        if (SW.getSwitch(4)) {
            x+=2;
        }
    }




    if (prev_x != x) {
        if(I2C2::getInstance().writeReg(0xaf,0x07,(uint8_t)x)) {
            //printf("X written ");
        } else {
            printf("X not written ");
        }
    }
    if (prev_y != y) {
        if(I2C2::getInstance().writeReg(0xaf,0x08,(uint8_t)y)) {
            //printf("Y written\n");
        } else {
            printf("Y not written\n");
        }
    }

    return true;
}

void i2c_master_task::setMode(Mode mode) {
    _mode = mode;
}


