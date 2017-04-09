/*
 * watchdog.hpp
 *
 *  Created on: Apr 5, 2017
 *      Author: James
 */

#ifndef L5_APPLICATION_WATCHDOG_HPP_
#define L5_APPLICATION_WATCHDOG_HPP_

#include "scheduler_task.hpp"
#include "soft_timer.hpp"

class Watchdog : public scheduler_task {
    SoftTimer CPUtimer;
public:
	Watchdog(uint8_t priority);
	bool init(void);
	bool run(void* p);
	void recordUsage();
};


#endif /* L5_APPLICATION_WATCHDOG_HPP_ */
