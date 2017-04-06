#ifndef _LIGHT_CONSUMER_HPP_
#define _LIGHT_CONSUMER_HPP_

#include "scheduler_task.hpp"
#include "FreeRTOS.h"
#include "event_groups.h"

class LightConsumer : public scheduler_task
{
public:
    const static uint8_t groupBit = (1 << 1);
    LightConsumer(uint8_t priority);
    bool init();
    bool run(void *param);
    ~LightConsumer();

private:
    QueueHandle_t lightValueQueue;
    EventGroupHandle_t eventGroup;
};

#endif