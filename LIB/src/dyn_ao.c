#include <stdio.h>
#include <string.h>

#include "dyn_ao.h"

#define DYNAMIC_AO_MAX_TASKS_COUNT 5
#define DYNAMIC_AO_QUEUE_LENGTH 8
#define DYNAMIC_AO_TASK_STACK_SIZE (2 * configMINIMAL_STACK_SIZE)
#define DYNAMIC_AO_TASK_PRIORIY (tskIDLE_PRIORITY + 1UL)


static void dynamic_ao_delete_task(DynamicAO* const ao)
{
	printf("[DYN AO] Deleting task \"%s\"\n", pcTaskGetName(NULL));
	ao->task_count--;
	vTaskDelete(NULL);
}


static void ao_task(void* parameters)
{
	DynamicAO* const AO = (DynamicAO*)(parameters);

    Event event;
    printf("[%s] Task Running\n", pcTaskGetName(NULL));

    while(1) {
        if(xQueueReceive(AO->queue, &event, 0) == pdPASS) {
            if(AO->dispatch_function) {
            	AO->dispatch_function(event.id, event.opt_data_address);
            }
        } else {
        	dynamic_ao_delete_task(AO);
        }
    }
}

static bool dynamic_ao_create_task(DynamicAO* const ao)
{
	if(ao->task_count < DYNAMIC_AO_MAX_TASKS_COUNT) {
		BaseType_t ret = xTaskCreate(
				ao_task,
				ao->task_name,
				DYNAMIC_AO_TASK_STACK_SIZE,
				(void*) ao,
				DYNAMIC_AO_TASK_PRIORIY,
				NULL);

		if(ret != pdPASS) {
			printf("[DYN AO] Error creating task \"%s\"\n", ao->task_name);
			return false;
		}

		ao->task_count++;
		return true;
	} else {
		printf("[DYN AO] Cannot allocate more \"%s\" tasks\n", ao->task_name);
		return false;
	}

}


bool dynamic_ao_initialize(DynamicAO* const ao, const char* task_name, dispatch_event_handler_t handler)
{
    if(ao == NULL) {
    	printf("[DYN AO] Received NULL AO\n");
    	return false;
    }

    if(handler == NULL) {
    	printf("[DYN AO] Received NULL handler\n");
    	return false;
    }

    ao->dispatch_function = handler;
    ao->task_count = 0;
    memcpy(ao->task_name, task_name, strlen(task_name));

    ao->queue = xQueueCreate(DYNAMIC_AO_QUEUE_LENGTH, sizeof(Event));
    if(ao->queue == NULL) {
    	printf("[DYN AO] Error creating queue for task \"%s\"\n", task_name);
    	return false;
    }

    printf("[DYN AO] Dynamic Active Object \"%s\" created!\n", task_name);
    return true;
}

bool dynamic_ao_send_event(DynamicAO* ao, Event* const event)
{
    if (xQueueSend(ao->queue, (void*)(event), 0) == pdPASS) {
    	if(ao->task_count == 0) {
    		dynamic_ao_create_task(ao);
    	}
    	return true;
    } else if(dynamic_ao_create_task(ao)) {
    	if(xQueueSend(ao->queue, (void*)(event), 0) == pdPASS) {
    		return true;
    	} else {
    		printf("[DYN AO] Error posting an event\n");
    	}
    } else {
        printf("[DYN AO] Cannot create more \"%s\" tasks\n", ao->task_name);
    }

    printf("[DYN AO] Error posting the event to \"%s\"'s queue\n", ao->task_name);
	return false;
}
