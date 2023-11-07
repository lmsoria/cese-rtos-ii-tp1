// ------ inclusions ---------------------------------------------------
#include <stdio.h>


#include "app_resources.h"

#include "SVC_led.h"

/// | Private typedef -----------------------------------------------------------
/// | Private define ------------------------------------------------------------

#define LED_AO_QUEUE_LENGTH 16
/// | Private macro -------------------------------------------------------------
/// | Private variables ---------------------------------------------------------
/// | Private function prototypes -----------------------------------------------

/// | Private functions ---------------------------------------------------------
void task_led(void* parameters)
{
    LEDActiveObject* const AO = (LEDActiveObject*)(parameters);

    printf("[%s] Task Created\n", pcTaskGetName(NULL));

	while(1) {}
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
