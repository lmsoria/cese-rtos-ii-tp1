// ------ inclusions ---------------------------------------------------
#include <stdio.h>
#include "cmsis_os.h"

#include "app_resources.h"

#include "SVC_button.h"

/// | Private typedef -----------------------------------------------------------

typedef enum
{
	DEBOUNCER_STATE_WAIT_PRESS = 0,    ///< Key released, waiting to be pressed
	DEBOUNCER_STATE_DEBOUNCE_ACTIVE,   ///< The key has been pressed and we need to filter the debouncing
	DEBOUNCER_STATE_WAIT_RELEASE,      ///< Key pressed, waiting to be released
	DEBOUNCER_STATE_DEBOUNCE_INACTIVE, ///< The key has been released, so we need to filter the debouncing
} DebouncerState;

typedef struct
{
	BoardButtons button;
	DebouncerState state;
    uint32_t timer_debounce;
    uint32_t timer_up;
} Debouncer;

/// | Private define ------------------------------------------------------------

#define DEBOUNCE_PERIOD_MS 40

/// | Private macro -------------------------------------------------------------
/// | Private variables ---------------------------------------------------------
/// | Private function prototypes -----------------------------------------------

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
			}
			break;
		default:
			debouncer.state = DEBOUNCER_STATE_WAIT_PRESS;
			break;
		}

		if(debouncer.state == DEBOUNCER_STATE_WAIT_RELEASE) {
			debouncer.timer_up++;
		} else {
			debouncer.timer_up = 0;
		}

		vTaskDelay(pdMS_TO_TICKS(1));
	}
}
