// ------ inclusions ---------------------------------------------------
#include <stdio.h>
#include "cmsis_os.h"

#include "app_resources.h"

#include "SVC_button.h"
#include "SVC_led.h"

/// | Private typedef -----------------------------------------------------------

typedef enum
{
	DEBOUNCER_STATE_WAIT_PRESS = 0,    ///< Key released, waiting to be pressed
	DEBOUNCER_STATE_DEBOUNCE_ACTIVE,   ///< The key has been pressed and we need to filter the debouncing
	DEBOUNCER_STATE_WAIT_RELEASE,      ///< Key pressed, waiting to be released
	DEBOUNCER_STATE_DEBOUNCE_INACTIVE, ///< The key has been released, so we need to filter the debouncing
} DebouncerState;


/// @brief Struct that applies an debouncing mechanism to an specific button.
typedef struct
{
	BoardButtons button;     ///< Button instance associated to the Debouncer
	DebouncerState state;    ///< Current debouncer state
    uint32_t timer_debounce; ///< Counter used for measuring debouncing transients
    uint32_t timer_up;       ///< Counter used for measuring pressed time.
} Debouncer;

/// @brief Events to be detected by the button task
typedef enum {
    EVENT_INITIAL, ///< Initial state.
    EVENT_SHORT,   ///< Detected when the button is being pressed in the range [EVENT_SHORT_THRESHOLD_MIN_MS, EVENT_LONG_THRESHOLD_MIN_MS)
    EVENT_LONG,    ///< Detected when the button is being pressed in the range [EVENT_LONG_THRESHOLD_MIN_MS, EVENT_BLOCKED_THRESHOLD_MIN_MS)
    EVENT_BLOCKED  ///< Detected when the button is being pressed in the range >= EVENT_BLOCKED_THRESHOLD_MIN_MS
} ButtonEvent;

/// | Private define ------------------------------------------------------------

#define DEBOUNCE_PERIOD_MS 40
#define EVENT_SHORT_THRESHOLD_MIN_MS 100
#define EVENT_LONG_THRESHOLD_MIN_MS 2000
#define EVENT_BLOCKED_THRESHOLD_MIN_MS 8000

/// | Private macro -------------------------------------------------------------
/// | Private variables ---------------------------------------------------------
extern LEDActiveObject led_ao_green;
/// | Private function prototypes -----------------------------------------------
static void process_button_timer_up(const uint32_t timer_up);

/// | Private functions ---------------------------------------------------------
void task_button(void* parameters)
{
	ButtonTaskData* const DATA = (ButtonTaskData*)(parameters);

	Debouncer debouncer =
	{
		.button = DATA->button,
		.state = DEBOUNCER_STATE_WAIT_PRESS,
		.timer_debounce = 0,
		.timer_up = 0,
	};

	printf("[%s] Task Created\n", pcTaskGetName(NULL));
	while(1) {
		switch(debouncer.state) {
		case DEBOUNCER_STATE_WAIT_PRESS:
			if(button_read(debouncer.button) == BUTTON_PRESSED) {
				debouncer.timer_debounce = pdMS_TO_TICKS(DEBOUNCE_PERIOD_MS);
				debouncer.state = DEBOUNCER_STATE_DEBOUNCE_ACTIVE;
			}
			break;
		case DEBOUNCER_STATE_DEBOUNCE_ACTIVE:
			if(button_read(debouncer.button) == BUTTON_RELEASED) {
				debouncer.state = DEBOUNCER_STATE_WAIT_PRESS;
			} else if(debouncer.timer_debounce) {
				debouncer.timer_debounce--;
			} else {
				debouncer.timer_up = 0;
				debouncer.state = DEBOUNCER_STATE_WAIT_RELEASE;
			}
			break;
		case DEBOUNCER_STATE_WAIT_RELEASE:
			if(button_read(debouncer.button) == BUTTON_RELEASED) {
				debouncer.timer_debounce = pdMS_TO_TICKS(DEBOUNCE_PERIOD_MS);
				debouncer.state = DEBOUNCER_STATE_DEBOUNCE_INACTIVE;
			}
			break;
		case DEBOUNCER_STATE_DEBOUNCE_INACTIVE:
			if(button_read(debouncer.button) == BUTTON_PRESSED) {
				debouncer.state = DEBOUNCER_STATE_WAIT_RELEASE;
			} else if(debouncer.timer_debounce) {
				debouncer.timer_debounce--;
			} else {
				debouncer.state = DEBOUNCER_STATE_WAIT_PRESS;
				printf("Button Released\n");
			}
			break;
		default:
			debouncer.state = DEBOUNCER_STATE_WAIT_PRESS;
			break;
		}

		if(debouncer.state == DEBOUNCER_STATE_WAIT_RELEASE) {
			debouncer.timer_up++;
			process_button_timer_up(debouncer.timer_up);
		} else {
			debouncer.timer_up = 0;
		}

		vTaskDelay(pdMS_TO_TICKS(1));
	}
}

static void process_button_timer_up(const uint32_t timer_up)
{
	static ButtonEvent current_event = EVENT_INITIAL;
	ButtonEvent new_event;

	if(timer_up >= EVENT_SHORT_THRESHOLD_MIN_MS && timer_up < EVENT_LONG_THRESHOLD_MIN_MS) {
		new_event = EVENT_SHORT;
	} else if(timer_up >= EVENT_LONG_THRESHOLD_MIN_MS && timer_up < EVENT_BLOCKED_THRESHOLD_MIN_MS) {
		new_event = EVENT_LONG;
	} else if(timer_up >= EVENT_BLOCKED_THRESHOLD_MIN_MS) {
		new_event = EVENT_BLOCKED;
	} else {
		new_event = EVENT_INITIAL;
	}

	if(new_event != current_event) {
		current_event = new_event;
		LEDEvent event_to_be_sent;

		switch(current_event) {
		case EVENT_SHORT:
			printf("SHORT\n");
			event_to_be_sent.type = LED_EVENT_TOGGLE;
			event_to_be_sent.led = LED1;
			led_ao_send_event(&led_ao_green, &event_to_be_sent);
			break;
		case EVENT_LONG:
            event_to_be_sent.type = LED_EVENT_TOGGLE;
            event_to_be_sent.led = LED3;
            led_ao_send_event(&led_ao_green, &event_to_be_sent);
			printf("LONG\n");
			break;
		case EVENT_BLOCKED:
			printf("BLOCKED\n");
			break;
		default:
			break;
		}
	}
}
