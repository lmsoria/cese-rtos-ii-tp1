/// | Includes ------------------------------------------------------------------
#include <stdio.h>

#include "ao.h"
#include "dyn_ao.h"

#include "SVC_button.h"
#include "SVC_led.h"
#include "SVC_sys.h"

/// | Private typedef -----------------------------------------------------------
/// | Private define ------------------------------------------------------------
/// | Private macro -------------------------------------------------------------
/// | Private variables ---------------------------------------------------------
static ActiveObject ao_sys;

/// | Exported variables --------------------------------------------------------
ActiveObject* const AO_SYS = &ao_sys;

extern DynamicAO* const AO_LED;

/// | Private function prototypes -----------------------------------------------

/// @brief Process events received on the AO queue
/// @param event
static void svc_sys_dispatch_event(Event* event);

/// @brief Send an event to the LED's Active Object. This function relies on knowing about the LED service.
/// @param event_id type of event to send
/// @param led led to modify
static void send_event(LEDEventType event_id, ApplicationLEDs led);

/// | Private functions ---------------------------------------------------------

bool svc_sys_initialize()
{
	return ao_initialize(AO_SYS, "sys", svc_sys_dispatch_event);
}

static void svc_sys_dispatch_event(Event* event)
{
    printf("[%s] Event Received: ", pcTaskGetName(NULL));

	switch ((ButtonEvent)(event->id)) {
	case BUTTON_EVENT_SHORT:
		printf("BUTTON_EVENT_SHORT\n");
		send_event(LED_EVENT_TOGGLE, LED_GREEN);
		break;
	case BUTTON_EVENT_LONG:
		printf("BUTTON_EVENT_LONG\n");
		send_event(LED_EVENT_TOGGLE, LED_RED);
		break;
	case BUTTON_EVENT_BLOCKED:
		printf("BUTTON_EVENT_BLOCKED\n");
		send_event(LED_EVENT_ON, LED_GREEN);
		send_event(LED_EVENT_ON, LED_RED);
		break;
	case BUTTON_EVENT_RELEASED:
		printf("BUTTON_EVENT_RELEASED\n");
		send_event(LED_EVENT_OFF, LED_GREEN);
		send_event(LED_EVENT_OFF, LED_RED);
		break;
	default:
		configASSERT(pdFAIL && "Invalid event");
		break;
	}
}

static void send_event(LEDEventType event_id, ApplicationLEDs led)
{
	Event event_to_be_sent =
	{
	    .id = (uint32_t)event_id,
        .opt_data_address = (void*)led,
	};

	if(!dynamic_ao_send_event(AO_LED, &event_to_be_sent)) {
		printf("[%s] Error sending event to the queue\n", pcTaskGetName(NULL));
	}
}

