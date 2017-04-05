#ifndef _LIGHT_PRODUCER_HPP_
#define _LIGHT_PRODUCER_HPP_

#include "scheduler_task.hpp"
#include "FreeRTOS.h"

typedef enum {
    shared_lightValues
} sharedHandle_t;

typedef struct {
    uint16_t lightLevelRaw;
    uint8_t lightLevelPercentage;
} lightvalue_t;

class LightProducer : public scheduler_task
{
public:
    LightProducer(uint8_t priority);
    bool init();
    bool run(void *param);
    ~LightProducer();

private:
    QueueHandle_t lightValueQueue;
};

#endif