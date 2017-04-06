/*
 * watchdog.cpp
 *
 *  Created on: Apr 5, 2017
 *      Author: James
 */

#include "watchdog.hpp"
#include <stdio.h>
#include "event_groups.h"



#define BIT_1    ( 1 << 1 )
#define BIT_2    ( 1 << 2 )

typedef enum {
    shared_eventGroupID
} sharedHandleID_t;


Watchdog::Watchdog(uint8_t priority) : scheduler_task("watchdog", 1024*4, priority) {
    // task runs once every second
    setRunDuration(1000);

}

bool Watchdog::init(void) {
    return true;
}

bool Watchdog::run(void *p) {
    EventGroupHandle_t taskEventGroup = getSharedObject(shared_eventGroupID);
    printf("test\n");
    static EventBits_t taskBits;
    taskBits = xEventGroupWaitBits(
                        taskEventGroup,    // The event group being tested.
                        BIT_1 | BIT_2,  // The bits within the event group to wait for.
                        pdTRUE,         // BIT_0 and BIT_4 should be cleared before returning.
                        pdTRUE,         // Wait for both bits
                        1000 );         // Wait a maximum of 1000ms for both bit to be set.

    if( ( taskBits & ( BIT_1 | BIT_2 ) ) == ( BIT_1 | BIT_2 ) )
    {
        // xEventGroupWaitBits() returned because both bits were set.
    }

    return true;
}
