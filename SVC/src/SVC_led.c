// ------ inclusions ---------------------------------------------------
#include <stdio.h>


#include "app_resources.h"


#include "HAL_led.h"
#include "SVC_led.h"

/// | Private typedef -----------------------------------------------------------
/// | Private define ------------------------------------------------------------

#define LED_AO_QUEUE_LENGTH 16
/// | Private macro -------------------------------------------------------------
/// | Private variables ---------------------------------------------------------
/// | Private function prototypes -----------------------------------------------
static void execute_event(LEDEvent event);
/// | Private functions ---------------------------------------------------------
void task_led(void* parameters)
{
    LEDActiveObject* const AO = (LEDActiveObject*)(parameters);

    LEDEvent event;
    printf("[%s] Task Created\n", pcTaskGetName(NULL));



	while(1) {
	    if(xQueueReceive(AO->queue, &event, portMAX_DELAY) == pdPASS) {
	        printf("[%s] Event Received: ", pcTaskGetName(NULL));
	        execute_event(event);
	    }
	}
}


void led_initialize_ao(LEDActiveObject* ao, const char* ao_task_name)
{
    BaseType_t ret;

    ret = xTaskCreate(
            task_led,
            ao_task_name,
            (2 * configMINIMAL_STACK_SIZE),
            (void*)ao,
            (tskIDLE_PRIORITY + 1UL),
            NULL
    );
    configASSERT(ret == pdPASS);

    ao->queue = xQueueCreate(LED_AO_QUEUE_LENGTH, sizeof(LEDEvent));
    configASSERT(ao->queue);
}

static void execute_event(LEDEvent event)
{
    switch (event) {
        case LED_EVENT_ON:
            printf("LED_EVENT_ON\n");
            led_set(LED1);
            break;
        case LED_EVENT_OFF:
            printf("LED_EVENT_OFF\n");
            led_clear(LED1);
            break;
        case LED_EVENT_TOGGLE:
            printf("LED_EVENT_TOGGLE\n");
            led_toggle(LED1);
            break;
        default:
            configASSERT(pdFAIL && "Invalid LED event");
            break;
    }
}

void led_ao_send_event(LEDActiveObject* ao, LEDEvent* const event)
{
    BaseType_t ret;

    ret = xQueueSend(ao->queue, (void*)(event), portMAX_DELAY);
    if(ret == pdPASS) {
        printf("LED event sent!\n");
    } else {
        printf("Error sending LED event\n");
    }
}

