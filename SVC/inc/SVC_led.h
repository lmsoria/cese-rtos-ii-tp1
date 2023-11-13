#pragma once

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "HAL_led.h"

typedef enum
{
    LED_EVENT_ON,     ///< Turn on a LED
    LED_EVENT_OFF,    ///< Turn off a LED
    LED_EVENT_TOGGLE, ///< Toggle a LED
} LEDEventType;

typedef enum
{
    LED_GREEN = LED1,
    LED_RED = LED3,
} ApplicationLEDs;

typedef struct
{
    ApplicationLEDs led;
    LEDEventType type;
} LEDEvent;

typedef struct
{
    QueueHandle_t queue;
    TaskHandle_t* task;
} LEDActiveObject;

void led_initialize_ao(LEDActiveObject* ao, const char* ao_task_name);

void led_ao_send_event(LEDActiveObject* ao, LEDEvent* const event);

void task_led(void* parameters);
