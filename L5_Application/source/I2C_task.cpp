
#include "I2C_task.hpp"
#include "stdint.h"
#include "I2C2.hpp"
#include <cstring>
#include <stdio.h>

volatile uint8_t i2c_task::memory[];

i2c_task::i2c_task(uint8_t priority) : scheduler_task("i2c", 2048, priority),tft(0, 0, 0, 0, 0) {
    setRunDuration(500); // 30 frames/second
    tft.reset();
    tft.initDisplay();
    tft.fillScreen(BLACK);
}

bool i2c_task::init(void) {
    I2C2::getInstance().init_slave(0xFA, (uint8_t*) memory, sizeof(memory));
    memset((uint8_t *)memory,0,sizeof(memory));
    return true;
}

bool i2c_task::run(void *p) {

    printf("memory [0] %x [1] %x [2] %x [3] %x\n",memory[0],memory[1],memory[2],memory[3]);

    if (memory[0] == 0) {

    } else {

    }
    static uint16_t x=0 , y=0;
    static uint16_t prev_x=0,prev_y=0;
    prev_x = x;
    prev_y = y;

    x = memory[1];
    y = memory[2];
    // erase prev rect
    tft.fillRoundRect((uint16_t)prev_x,(uint16_t)prev_y,10,10,4,BLACK);
    //draw new rect
    tft.fillRoundRect((uint16_t)x,(uint16_t)y,10,10,4,WHITE);
    return true;
}
// pg 466 in LPC datasheet

void i2c_task::writeReg() {

}
