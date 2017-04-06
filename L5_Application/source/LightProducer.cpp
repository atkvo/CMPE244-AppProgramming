#include "LightProducer.hpp"
#include "light_sensor.hpp"
// #include "acceleration_sensor.hpp"

LightProducer::LightProducer(uint8_t priority) :
        scheduler_task("lightproducer", 1024, priority),
        samplesCollected(0)
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

    samples[samplesCollected] = result;
    samplesCollected++;
    
    if (samplesCollected >= LightProducer::MAX_SAMPLES) {
        lightvalue_t average;
        averageSamples(&average);
        xQueueSend(getSharedObject(shared_lightValues), &average, portMAX_DELAY);
    }

    xEventGroupSetBits(eventGroup, LightProducer::groupBit);
    return true;
}

void LightProducer::averageSamples(lightvalue_t * result)
{
    uint16_t averageRaw = 0;
    uint8_t averagePercent = 0;
    for (int i = 0; i < LightProducer::MAX_SAMPLES; i++)
    {
        averageRaw += samples[i].lightLevelRaw;
        averagePercent += samples[i].lightLevelPercentage;
    }

    averageRaw = averageRaw / LightProducer::MAX_SAMPLES;
    averagePercent = averagePercent / LightProducer::MAX_SAMPLES;

    result->lightLevelRaw = averageRaw;
    result->lightLevelPercentage = averagePercent;
}

LightProducer::~LightProducer()
{

}