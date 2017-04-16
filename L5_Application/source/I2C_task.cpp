
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
    static float x=20.0f , y=20.0f;
    x += 0.1;
    y += 0.1;
    tft.fillRoundRect((uint16_t)x,(uint16_t)y,10,10,4,WHITE);
    return true;
}
// pg 466 in LPC datasheet

void i2c_task::writeReg() {

}
