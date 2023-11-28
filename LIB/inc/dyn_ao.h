#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "event.h"

#define DYN_AO_MAX_TASKS_COUNT 5
#define DYN_AO_TASK_NAME_LENGHT 16

typedef struct {
    QueueHandle_t queue;
    TaskHandle_t* task[DYN_AO_MAX_TASKS_COUNT];
    dispatch_event_handler_t dispatch_function;
    size_t task_count;
    size_t client_count;
    char task_name[DYN_AO_TASK_NAME_LENGHT];
} DynamicAO;

bool dynamic_ao_initialize(DynamicAO* const ao, const char* task_name, dispatch_event_handler_t handler);


bool dynamic_ao_send_event(DynamicAO* const ao, Event* const event);
