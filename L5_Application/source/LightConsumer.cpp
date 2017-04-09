#include "LightConsumer.hpp"
#include "LightProducer.hpp"
#include "storage.hpp"
#include "rtc.h"
#include <stdio.h>
#include <string.h>

LightConsumer::LightConsumer(uint8_t priority) :
        scheduler_task("lightcon", 2 * 1024, priority)
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
    char time[64] = {0};
    const char * time_uncorrected = rtc_get_date_time_str();
    strncpy(time,time_uncorrected , strlen(time_uncorrected)-1);  // remove the newline from the end of the time string
    int len = sprintf(buf, "%s, %i\n", time, result.lightLevelRaw);
    Storage::append("1:sensor.txt", buf, len, 0);
    
    //printf("time, %i\n", result.lightLevelRaw);

    xEventGroupSetBits(eventGroup, LightConsumer::groupBit);
    return true;
}

LightConsumer::~LightConsumer()
{

}
