
#include "I2C_task.hpp"
#include "stdint.h"
#include "I2C2.hpp"
#include <cstring>
#include <stdio.h>
#include "printf_lib.h"
#include "LPC17xx.h"
#include "utilities.h"

volatile uint8_t i2c_task::memory[];

i2c_task::i2c_task(uint8_t priority) : scheduler_task("i2c", 2048, priority),tft(0, 0, 0, 0, 0) {
    setRunDuration(10); // 30 frames/second
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

    for (int i = 0 ; i < 16; i ++) {
        u0_dbg_printf("[%2d] %2x ",i,memory[i]);
    }
    u0_dbg_printf("\n");
    u0_dbg_printf("MM Control Reg: %x  Address Reg: %x\n",LPC_I2C2->MMCTRL, LPC_I2C2->I2ADR0);

    if (memory[6] == 0xAA) {
//        LPC_I2C2->MMCTRL = 0x01;
        LPC_I2C2->I2CONCLR = 0x40;
        u0_dbg_printf("***Monitor Mode\n");
        delay_ms(200);
//        LPC_I2C2->MMCTRL = 0x00;
        LPC_I2C2->I2CONSET = 0x44;

        memory[6] = 0xBB;
        u0_dbg_printf("Normal Mode\n");
    }
    static uint16_t x=0 , y=0;
    static uint16_t prev_x=0,prev_y=0;
    prev_x = x;
    prev_y = y;

    x = memory[7];
    y = memory[8];
    // erase prev rect
    tft.fillRoundRect((uint16_t)prev_x,(uint16_t)prev_y,10,10,4,BLACK);
    //draw new rect
    tft.fillRoundRect((uint16_t)x,(uint16_t)y,10,10,4,WHITE);
    return true;
}
// pg 466 in LPC datasheet

void i2c_task::writeReg() {

}
