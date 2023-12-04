// ------ inclusions ---------------------------------------------------
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
static void svc_sys_dispatch_event(uint32_t event_type, void* unused);

/// | Private functions ---------------------------------------------------------

bool svc_sys_initialize()
{
	return ao_initialize(AO_SYS, "sys", svc_sys_dispatch_event);
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
		dynamic_ao_send_event(AO_LED, &event_to_be_sent);
		break;
	case BUTTON_EVENT_LONG:
		printf("BUTTON_EVENT_LONG\n");
		event_to_be_sent.id = (uint32_t)(LED_EVENT_TOGGLE);
		event_to_be_sent.opt_data_address = (void*)(LED_RED);
		dynamic_ao_send_event(AO_LED, &event_to_be_sent);
		break;
	case BUTTON_EVENT_BLOCKED:
		printf("BUTTON_EVENT_BLOCKED\n");
		event_to_be_sent.id = (uint32_t)(LED_EVENT_ON);
		event_to_be_sent.opt_data_address = (void*)(LED_GREEN);
		dynamic_ao_send_event(AO_LED, &event_to_be_sent);
		event_to_be_sent.opt_data_address = (void*)(LED_RED);
		dynamic_ao_send_event(AO_LED, &event_to_be_sent);
		break;
	case BUTTON_EVENT_RELEASED:
		printf("BUTTON_EVENT_RELEASED\n");
		event_to_be_sent.id = (uint32_t)(LED_EVENT_OFF);
		event_to_be_sent.opt_data_address = (void*)(LED_GREEN);
		dynamic_ao_send_event(AO_LED, &event_to_be_sent);
		event_to_be_sent.opt_data_address = (void*)(LED_RED);
		dynamic_ao_send_event(AO_LED, &event_to_be_sent);
		break;
	default:
		configASSERT(pdFAIL && "Invalid event");
		break;
	}
}
