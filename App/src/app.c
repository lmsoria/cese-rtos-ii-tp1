#include <stdio.h>

#include "ao.h"

#include "app.h"
#include "app_resources.h"

#include "SVC_led.h"
#include "SVC_button.h"

/// | Private typedef -----------------------------------------------------------
/// | Private define ------------------------------------------------------------
/// | Private macro -------------------------------------------------------------
/// | Private function prototypes -----------------------------------------------
/// | Private variables ---------------------------------------------------------
/// | Exported variables -------------------------------------------------------

// Data passed to future tasks. TODO: List available tasks inside an enum so we don't hardcode the indices
static const ButtonTaskData BUTTON_TASK_DATA_ARRAY[1] =
{
    [0] = {
            .button = USER_BUTTON,
          },
};

/// | Exported variables --------------------------------------------------------
TaskHandle_t button_task_handle;
ActiveObject ao_led;


/// | Private functions ---------------------------------------------------------

void app_init()
{
    BaseType_t ret;

    printf("Main application starts here\n");

    // Initialize LED Active Object
    if(!svc_led_initialize(&ao_led, "led")) {
    	printf("Error: Couldn't initialize led AO\n");
    	configASSERT(false);
    }

    // Create button task
    ret = xTaskCreate(
            task_button,
            "Task Button",
            (2 * configMINIMAL_STACK_SIZE),
            (void*) &BUTTON_TASK_DATA_ARRAY[0],
            (tskIDLE_PRIORITY + 1UL),
            &button_task_handle);
    configASSERT(ret == pdPASS);
}
