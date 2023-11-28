// ------ inclusions ---------------------------------------------------
#include <stdio.h>

#include "app_resources.h"

#include "SVC_button.h"
#include "SVC_led.h"
#include "SVC_sys.h"

/// | Private typedef -----------------------------------------------------------
/// | Private define ------------------------------------------------------------
/// | Private macro -------------------------------------------------------------
/// | Private variables ---------------------------------------------------------
/// | Private function prototypes -----------------------------------------------

/// @brief Process events received on the AO queue
/// @param event
static void svc_sys_dispatch_event(uint32_t event_type, void* unused);

/// | Private functions ---------------------------------------------------------

bool svc_sys_initialize(ActiveObject* const ao, const char* ao_task_name)
{
	return ao_initialize(ao, ao_task_name, svc_sys_dispatch_event);
}

static void svc_sys_dispatch_event(uint32_t event_type, void* unused)
{
    Event event_to_be_sent;

    printf("[%s] Event Received: ", pcTaskGetName(NULL));


	switch ((ButtonEvent)(event_type)) {
	case BUTTON_EVENT_SHORT:
		printf("BUTTON_EVENT_SHORT\n");
		event_to_be_sent.id = (uint32_t)(LED_EVENT_TOGGLE);
		event_to_be_sent.opt_data_address = (void*)(LED_GREEN);
		dynamic_ao_send_event(&ao_led, &event_to_be_sent);
		break;
	case BUTTON_EVENT_LONG:
		printf("BUTTON_EVENT_LONG\n");
		event_to_be_sent.id = (uint32_t)(LED_EVENT_TOGGLE);
		event_to_be_sent.opt_data_address = (void*)(LED_RED);
		dynamic_ao_send_event(&ao_led, &event_to_be_sent);
		break;
	case BUTTON_EVENT_BLOCKED:
		printf("BUTTON_EVENT_BLOCKED\n");
		event_to_be_sent.id = (uint32_t)(LED_EVENT_ON);
		event_to_be_sent.opt_data_address = (void*)(LED_GREEN);
		dynamic_ao_send_event(&ao_led, &event_to_be_sent);
		event_to_be_sent.opt_data_address = (void*)(LED_RED);
		dynamic_ao_send_event(&ao_led, &event_to_be_sent);
		break;
	case BUTTON_EVENT_RELEASED:
		printf("BUTTON_EVENT_RELEASED\n");
		event_to_be_sent.id = (uint32_t)(LED_EVENT_OFF);
		event_to_be_sent.opt_data_address = (void*)(LED_GREEN);
		dynamic_ao_send_event(&ao_led, &event_to_be_sent);
		event_to_be_sent.opt_data_address = (void*)(LED_RED);
		dynamic_ao_send_event(&ao_led, &event_to_be_sent);
		break;
	default:
		configASSERT(pdFAIL && "Invalid event");
		break;
	}
}
