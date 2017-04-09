#include "LightConsumer.hpp"
#include "LightProducer.hpp"
#include "storage.hpp"
#include "rtc.h"
#include <stdio.h>

LightConsumer::LightConsumer(uint8_t priority) :
        scheduler_task("consumeroflight", 1024, priority)
{

}

bool LightConsumer::init(void)
{
    /* Nothing to initialize */
    return true;
}

bool LightConsumer::run(void *param)
{
    lightvalue_t result;
    QueueHandle_t lightQueue = getSharedObject(shared_lightValues);
    if (lightQueue == NULL) {
        return false;
    }

    eventGroup = getSharedObject(shared_eventGroup);
    if (eventGroup == NULL) {
        return false;
    }

    xQueueReceive(lightQueue, &result, portMAX_DELAY);
    
    char buf[256] = { 0 };
    int len = sprintf(buf, "%s, %i\n", rtc_get_date_time_str(), result.lightLevelRaw);
    Storage::append("1:sensor.txt", buf, len, 0);
    
    printf("time, %i\n", result.lightLevelRaw);

    xEventGroupSetBits(eventGroup, LightConsumer::groupBit);
    return true;
}

LightConsumer::~LightConsumer()
{

}