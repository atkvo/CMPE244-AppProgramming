/*
 * I2C_task.hpp
 *
 *  Created on: Apr 11, 2017
 *      Author: James
 */

#ifndef L5_APPLICATION_SOURCE_I2C_TASK_HPP_
#define L5_APPLICATION_SOURCE_I2C_TASK_HPP_

#include "scheduler_task.hpp"

// Color definitions
#define BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF

#include "TFTLCD.h"

class i2c_task : public scheduler_task {

    enum {
        X_val,
        Y_val,
        Color
    };

    TFTLCD tft;
public:

    // first step is getting slave to recognize its own addresss
    volatile static uint8_t memory[16];

    i2c_task(uint8_t priority);
    bool init(void);
    bool run(void *p);
    // pg 466 in LPC datasheet

    void writeReg();

};



#endif /* L5_APPLICATION_SOURCE_I2C_TASK_HPP_ */
