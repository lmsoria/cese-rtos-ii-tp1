#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "event.h"

#define DYNAMIC_AO_TASK_NAME_LENGHT 16

typedef struct {
	char task_name[DYNAMIC_AO_TASK_NAME_LENGHT];
    QueueHandle_t queue;
    dispatch_event_handler_t dispatch_function;
    size_t task_count;
} DynamicAO;

bool dynamic_ao_initialize(DynamicAO* const ao, const char* task_name, dispatch_event_handler_t handler);

bool dynamic_ao_send_event(DynamicAO* const ao, Event* const event);
