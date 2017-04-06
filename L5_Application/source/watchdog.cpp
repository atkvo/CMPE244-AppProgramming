/*
 * watchdog.cpp
 *
 *  Created on: Apr 5, 2017
 *      Author: James
 */

#include "watchdog.hpp"
#include <stdio.h>
#include "event_groups.h"
#include "storage.hpp"
#include "string.h"

#define BIT_0    ( 1 << 0 )
#define BIT_1    ( 1 << 1 )

typedef enum {
    shared_lightValues,
    shared_eventGroup
} sharedHandle_t;

Watchdog::Watchdog(uint8_t priority) : scheduler_task("watchdog", 1024*4, priority) {
    // task runs once every second
    //setRunDuration(1000);

}

bool Watchdog::init(void) {
    return true;
}

bool Watchdog::run(void *p) {
    EventGroupHandle_t taskEventGroup = getSharedObject(shared_eventGroup);
    //printf("test\n");
    static EventBits_t taskBits;
    const int maxBufferSize = 256;
    static char stuckBuffer[maxBufferSize];
    memset(stuckBuffer, 0, maxBufferSize);



    taskBits = xEventGroupWaitBits(
                        taskEventGroup,    // The event group being tested.
                        BIT_0 | BIT_1,  // The bits within the event group to wait for.
                        pdTRUE,         // BIT_0 and BIT_1 should be cleared before returning.
                        pdTRUE,         // Wait for both bits
                        1000 );         // Wait a maximum of 1000ms for both bit to be set.

    if( ( taskBits & ( BIT_0 | BIT_1 ) ) != ( BIT_0 | BIT_1 ) )
    {
        // xEventGroupWaitBits() returned even though both bits were not set.
        if (( taskBits & BIT_0 ) != BIT_0) {
            strcpy(stuckBuffer,"Task 1 is stuck\n");
        }
        if (( taskBits & BIT_1 ) != BIT_1) {
            strcpy(stuckBuffer,"Task 2 is stuck\n");
        }
        if (( taskBits & BIT_1 ) != BIT_1) {
            strcpy(stuckBuffer,"Both tasks are stuck!\n");
        }
        Storage::append("1:stuck.txt", stuckBuffer, strlen(stuckBuffer));
    }

    return true;
}

void Watchdog::recordUsage() {
    const int delayInMs = (int)cmdParams;  // cast parameter str to integer

    if(delayInMs > 0) {
        vTaskResetRunTimeStats();
        vTaskDelayMs(delayInMs);
    }

    // Enum to char : eRunning, eReady, eBlocked, eSuspended, eDeleted
    const char * const taskStatusTbl[] = { "RUN", "RDY", "BLK", "SUS", "DEL" };

    // Limit the tasks to avoid heap allocation.
    const unsigned portBASE_TYPE maxTasks = 16;
    TaskStatus_t status[maxTasks];
    uint32_t totalRunTime = 0;
    uint32_t tasksRunTime = 0;
    const unsigned portBASE_TYPE uxArraySize =
            uxTaskGetSystemState(&status[0], maxTasks, &totalRunTime);

    output.printf("%10s Sta Pr Stack CPU%%          Time\n", "Name");
    for(unsigned priorityNum = 0; priorityNum < configMAX_PRIORITIES; priorityNum++)
    {
        /* Print in sorted priority order */
        for (unsigned i = 0; i < uxArraySize; i++) {
            TaskStatus_t *e = &status[i];
            if (e->uxBasePriority == priorityNum) {
                tasksRunTime += e->ulRunTimeCounter;

                const uint32_t cpuPercent = (0 == totalRunTime) ? 0 : e->ulRunTimeCounter / (totalRunTime/100);
                const uint32_t timeUs = e->ulRunTimeCounter;
                const uint32_t stackInBytes = (4 * e->usStackHighWaterMark);

                output.printf("%10s %s %2u %5u %4u %10u us\n",
                              e->pcTaskName, taskStatusTbl[e->eCurrentState], e->uxBasePriority,
                              stackInBytes, cpuPercent, timeUs);
            }
        }
    }
}

