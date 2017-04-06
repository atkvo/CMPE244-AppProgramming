#include "LightProducer.hpp"
#include "light_sensor.hpp"
// #include "acceleration_sensor.hpp"

LightProducer::LightProducer(uint8_t priority) :
        scheduler_task("lightproducer", 1024, priority)
{

}

bool LightProducer::init(void)
{
    const uint16_t POLL_RATE_MS = 1;
    lightValueQueue = xQueueCreate(10, sizeof(lightvalue_t));
    if (lightValueQueue == NULL)
    {
        return false;
    }

    eventGroup = xEventGroupCreate();
    if (eventGroup == NULL) {
        return false;
    }

    addSharedObject(shared_lightValues, lightValueQueue);
    addSharedObject(shared_eventGroup, eventGroup);
    setRunDuration(POLL_RATE_MS);
    return Light_Sensor::getInstance().init();
}

bool LightProducer::run(void *param)
{
    lightvalue_t result;
    result.lightLevelRaw = Light_Sensor::getInstance().getRawValue();
    result.lightLevelPercentage = Light_Sensor::getInstance().getPercentValue();

    xQueueSend(getSharedObject(shared_lightValues), &result, portMAX_DELAY);
    xEventGroupSetBits(eventGroup, LightProducer::groupBit);
    return true;
}

LightProducer::~LightProducer()
{

}