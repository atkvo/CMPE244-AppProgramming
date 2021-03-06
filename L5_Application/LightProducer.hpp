#ifndef _LIGHT_PRODUCER_HPP_
#define _LIGHT_PRODUCER_HPP_

#include "scheduler_task.hpp"
#include "FreeRTOS.h"
#include "event_groups.h"

typedef enum {
    shared_lightValues,
    shared_eventGroup
} sharedHandle_t;

typedef struct {
    uint16_t lightLevelRaw;
    uint8_t lightLevelPercentage;
} lightvalue_t;

class LightProducer : public scheduler_task
{
public:
    const static uint8_t groupBit = (1 << 0);
    const static uint8_t MAX_SAMPLES = 100;
    lightvalue_t samples[MAX_SAMPLES];
    LightProducer(uint8_t priority);
    bool init();
    bool run(void *param);
    ~LightProducer();

private:
    QueueHandle_t lightValueQueue;
    EventGroupHandle_t eventGroup;
    uint8_t samplesCollected;
    void averageSamples(lightvalue_t * result);
};

#endif