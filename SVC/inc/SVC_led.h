#pragma once

#include "FreeRTOS.h"
#include "queue.h"

#include "HAL_led.h"

typedef enum
{
    LED_EVENT_ON,
    LED_EVENT_OFF,
    LED_EVENT_TOGGLE,
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
} LEDActiveObject;

void led_initialize_ao(LEDActiveObject* ao, const char* ao_task_name);

void led_ao_send_event(LEDActiveObject* ao, LEDEvent* const event);

void task_led(void* parameters);
