/// | Includes ------------------------------------------------------------------
#include <stdio.h>

#include "ao.h"

#include "HAL_button.h"
#include "SVC_button.h"
#include "SVC_sys.h"

/// | Private typedef -----------------------------------------------------------

/// @brief Debouncer FSM States
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

/// | Private define ------------------------------------------------------------

#define DEBOUNCE_PERIOD_MS 40
#define EVENT_SHORT_THRESHOLD_MIN_MS 100
#define EVENT_LONG_THRESHOLD_MIN_MS 2000
#define EVENT_BLOCKED_THRESHOLD_MIN_MS 8000

/// | Private macro -------------------------------------------------------------
/// | Private variables ---------------------------------------------------------
extern ActiveObject* const AO_SYS;
/// | Private function prototypes -----------------------------------------------

static void task_button(void* unused);

/// @brief Process the "button pressed" action, which happens whenever the Debouncer is at DEBOUNCER_STATE_WAIT_RELEASE state.
///        Use this function to propagate events to other actors.
/// @param current_event current ButtonEvent. The function will modify its content (iif it's different from the previous one).
/// @param timer_up time that the button has been pressed, in ms
static void process_button_pressed_state(ButtonEvent* const current_event, const uint32_t timer_up);

/// @brief Process the "button released" action, which happens whenever the Debouncer is at DEBOUNCER_STATE_DEBOUNCE_INACTIVE state.
///        Use this function to propagate events to other actors.
/// @param current_event current ButtonEvent. The function won't modify its content.
static void process_button_released_state(ButtonEvent* const current_event);

/// | Private functions ---------------------------------------------------------

bool svc_button_initialize()
{
	BaseType_t ret = pdFALSE;
    // Create button task
    ret = xTaskCreate(
            task_button,
            "button",
            (2 * configMINIMAL_STACK_SIZE),
            NULL,
            (tskIDLE_PRIORITY + 1UL),
            NULL);

    return (ret == pdTRUE);
}

static void task_button(void* unused)
{
    Debouncer debouncer =
    {
        .button = USER_BUTTON,
        .state = DEBOUNCER_STATE_WAIT_PRESS,
        .timer_debounce = 0,
        .timer_up = 0,
    };

    ButtonEvent current_event = BUTTON_EVENT_INITIAL;

    printf("[%s] Task Created\n", pcTaskGetName(NULL));

    // Basic flow:
    // 1. Read button
    // 2. Debounce its state by using a Debouncer FSM
    // 3. If button is pressed, call process_button_pressed_state()
    // 4. If button released, call process_button_released_state()
    while (1) {
        switch (debouncer.state) {
        case DEBOUNCER_STATE_WAIT_PRESS:
            if (button_read(debouncer.button) == BUTTON_PRESSED) {
                debouncer.timer_debounce = pdMS_TO_TICKS(DEBOUNCE_PERIOD_MS);
                debouncer.state = DEBOUNCER_STATE_DEBOUNCE_ACTIVE;
            }
            break;
        case DEBOUNCER_STATE_DEBOUNCE_ACTIVE:
            if (button_read(debouncer.button) == BUTTON_RELEASED) {
                debouncer.state = DEBOUNCER_STATE_WAIT_PRESS;
            } else if (debouncer.timer_debounce) {
                debouncer.timer_debounce--;
            } else {
                debouncer.timer_up = 0;
                debouncer.state = DEBOUNCER_STATE_WAIT_RELEASE;
            }
            break;
        case DEBOUNCER_STATE_WAIT_RELEASE:
            if (button_read(debouncer.button) == BUTTON_RELEASED) {
                debouncer.timer_debounce = pdMS_TO_TICKS(DEBOUNCE_PERIOD_MS);
                debouncer.state = DEBOUNCER_STATE_DEBOUNCE_INACTIVE;
            }
            break;
        case DEBOUNCER_STATE_DEBOUNCE_INACTIVE:
            if (button_read(debouncer.button) == BUTTON_PRESSED) {
                debouncer.state = DEBOUNCER_STATE_WAIT_RELEASE;
            } else if (debouncer.timer_debounce) {
                debouncer.timer_debounce--;
            } else {
                debouncer.state = DEBOUNCER_STATE_WAIT_PRESS;
                process_button_released_state(&current_event);
            }
            break;
        default:
            debouncer.state = DEBOUNCER_STATE_WAIT_PRESS;
            break;
        }

        if (debouncer.state == DEBOUNCER_STATE_WAIT_RELEASE) {
            process_button_pressed_state(&current_event, ++debouncer.timer_up);
        } else {
            debouncer.timer_up = 0;
        }

        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

static void process_button_pressed_state(ButtonEvent* const current_event, const uint32_t timer_up)
{
    ButtonEvent new_event;

    const char* BUTTON_TASK_NAME = pcTaskGetName(NULL);

    if (timer_up >= EVENT_SHORT_THRESHOLD_MIN_MS && timer_up < EVENT_LONG_THRESHOLD_MIN_MS) {
        new_event = BUTTON_EVENT_SHORT;
    } else if (timer_up >= EVENT_LONG_THRESHOLD_MIN_MS && timer_up < EVENT_BLOCKED_THRESHOLD_MIN_MS) {
        new_event = BUTTON_EVENT_LONG;
    } else if (timer_up >= EVENT_BLOCKED_THRESHOLD_MIN_MS) {
        new_event = BUTTON_EVENT_BLOCKED;
    } else {
        new_event = BUTTON_EVENT_INITIAL;
    }

    // Since this function is being called periodically, we need to keep track of the new event and only send events
    // when there is a difference with the previous one.
    if (new_event != *current_event) {
        *current_event = new_event;
        Event event_to_be_sent;
        event_to_be_sent.opt_data_address = NULL;

        switch (*current_event) {
        case BUTTON_EVENT_SHORT:
            printf("[%s] Detected SHORT press\n", BUTTON_TASK_NAME);
            event_to_be_sent.id = (uint32_t)(BUTTON_EVENT_SHORT);
            ao_send_event(AO_SYS, &event_to_be_sent);
            break;

        case BUTTON_EVENT_LONG:
        	printf("[%s] Detected LONG press\n", BUTTON_TASK_NAME);
            event_to_be_sent.id = (uint32_t)(BUTTON_EVENT_LONG);
            ao_send_event(AO_SYS, &event_to_be_sent);
            break;

        case BUTTON_EVENT_BLOCKED:
            printf("[%s] Detected BLOCKED press\n", BUTTON_TASK_NAME);
            event_to_be_sent.id = (uint32_t)(BUTTON_EVENT_BLOCKED);
            ao_send_event(AO_SYS, &event_to_be_sent);
            break;

        default:
            break;
        }
    }
}

static void process_button_released_state(ButtonEvent* const current_event)
{
	Event event_to_be_sent;

    printf("[%s] Button Released\n", pcTaskGetName(NULL));

    switch (*current_event) {
    case BUTTON_EVENT_SHORT:
        break;

    case BUTTON_EVENT_LONG:
        break;

    case BUTTON_EVENT_BLOCKED:
        // As per design, only turn off the LEDs when the current state is BLOCKED
        event_to_be_sent.id = (uint32_t)(BUTTON_EVENT_RELEASED);
        ao_send_event(AO_SYS, &event_to_be_sent);
        break;

    default:
        break;
    }
}
