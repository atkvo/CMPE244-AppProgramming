/*
 * i2c_master_task.hpp
 *
 *  Created on: Apr 16, 2017
 *      Author: James
 */

#ifndef L5_APPLICATION_I2C_MASTER_TASK_HPP_
#define L5_APPLICATION_I2C_MASTER_TASK_HPP_

#include "scheduler_task.hpp"

class i2c_master_task : public scheduler_task {
public:
    i2c_master_task(uint8_t priority);
    bool init(void);
    bool run(void *p);
};


#endif /* L5_APPLICATION_I2C_MASTER_TASK_HPP_ */
