// ------ inclusions ---------------------------------------------------
#include <stdio.h>

#include "dyn_ao.h"

#include "HAL_led.h"
#include "SVC_led.h"

/// | Private typedef -----------------------------------------------------------
/// | Private define ------------------------------------------------------------
/// | Private macro -------------------------------------------------------------
/// | Private variables ---------------------------------------------------------
static DynamicAO ao_led;

/// | Exported variables --------------------------------------------------------
DynamicAO* const AO_LED = &ao_led;

/// | Private function prototypes -----------------------------------------------

/// @brief Process events received on the AO queue
/// @param event
static void svc_led_dispatch_event(Event* event);

/// | Private functions ---------------------------------------------------------

bool svc_led_initialize()
{
	return dynamic_ao_initialize(AO_LED, "led", svc_led_dispatch_event);
}

static void svc_led_dispatch_event(Event* event)
{
    printf("[%s] Event Received: ", pcTaskGetName(NULL));
    const ApplicationLEDs LED = (ApplicationLEDs)(event->opt_data_address);

    switch ((LEDEventType)(event->id)) {
    case LED_EVENT_ON:
        printf("LED_EVENT_ON\n");
        led_set(LED);
        break;
    case LED_EVENT_OFF:
        printf("LED_EVENT_OFF\n");
        led_clear(LED);
        break;
    case LED_EVENT_TOGGLE:
        printf("LED_EVENT_TOGGLE\n");
        led_toggle(LED);
        break;
    default:
        configASSERT(pdFAIL && "Invalid LED event");
        break;
    }
}
