/*
 * I2C_task.hpp
 *
 *  Created on: Apr 11, 2017
 *      Author: James
 */

#ifndef L5_APPLICATION_SOURCE_I2C_TASK_HPP_
#define L5_APPLICATION_SOURCE_I2C_TASK_HPP_


class i2c : public scheduler_task {
    // first step is getting slave to recognize its own addresss
    volatile static uint8_t memory[4] = { 0 };
public:

    i2c(uint8_t priority) : scheduler_task("i2c", 512, priority) {

    }

    bool init(void) {
        I2C2::getInstance().init_slave(0xDD, memory, sizeof(memory));
        return true;
    }

    bool run(void *p) {
        if (memory[0] == 0) {

        } else {

        }
        return true;
    }
    // pg 466 in LPC datasheet

    void writeReg() {


    }

    void init_slave()
}



#endif /* L5_APPLICATION_SOURCE_I2C_TASK_HPP_ */
