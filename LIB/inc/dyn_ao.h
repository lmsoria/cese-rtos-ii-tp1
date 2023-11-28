#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "event.h"

#define DYNAMIC_AO_TASK_NAME_LENGHT 16

/// @brief Dynamic Active Object struct. This struct will instantiate processing tasks on-demand 
///        (ie only when events are being posted). If there's no events on the queue, there won't be
///        any running tasks, "reducing CPU usage"
typedef struct {
	char task_name[DYNAMIC_AO_TASK_NAME_LENGHT]; ///< Name of the tasks that will be created.
    QueueHandle_t queue;                         ///< Event Queue.
    dispatch_event_handler_t dispatch_function;  ///< Callback that will be triggered on each event reception
    size_t task_count;                           ///< Number of running tasks.
} DynamicAO;

/// @brief Initialize a dynamic active object. This function won't start any thread, since they will
///        be created on demand.
/// @param ao Active Object to be initialized.
/// @param task_name Name of the running threads.
/// @param handler callback function used for processing incoming events.
/// @return true if the Active Object was initialized successfully, false otherwise
bool dynamic_ao_initialize(DynamicAO* const ao, const char* task_name, dispatch_event_handler_t handler);

/// @brief Post an Event to the Active Object's queue. If there's no threads running,
///        will create a new one.
/// @param ao Receiver of the even.
/// @param event Event to be sent.
/// @return true if the event was pushed into the queue, false otherwise.
bool dynamic_ao_send_event(DynamicAO* const ao, Event* const event);
