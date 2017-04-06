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
    shared_eventGroupID
} sharedHandleID_t;


Watchdog::Watchdog(uint8_t priority) : scheduler_task("watchdog", 1024*4, priority) {
    // task runs once every second
    //setRunDuration(1000);

}

bool Watchdog::init(void) {
    return true;
}

bool Watchdog::run(void *p) {
    EventGroupHandle_t taskEventGroup = getSharedObject(shared_eventGroupID);
    printf("test\n");
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
        Storage::append("stuck.txt", stuckBuffer, strlen(stuckBuffer));
    }

    return true;
}
