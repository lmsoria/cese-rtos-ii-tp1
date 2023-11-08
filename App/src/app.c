#include <stdio.h>

#include "cmsis_os.h"

#include "app.h"
#include "app_resources.h"

#include "SVC_button.h"

/// | Private typedef -----------------------------------------------------------
/// | Private define ------------------------------------------------------------
/// | Private macro -------------------------------------------------------------
/// | Private function prototypes -----------------------------------------------

/// | Private variables ---------------------------------------------------------

// Data passed to future tasks. TODO: List available tasks inside an enum so we don't hardcode the indices
static const ButtonTaskData BUTTON_TASK_DATA_ARRAY[1] =
{
    [0] = {
        .button = USER_BUTTON,
    },
};

/// | Exported variables --------------------------------------------------------
TaskHandle_t button_task_handle;

/// | Private functions ---------------------------------------------------------

void app_init()
{
	BaseType_t ret;

    printf("Main application starts here\n");

    // Create button task
    ret = xTaskCreate(
        task_button,
        "Task Button",
        (2 * configMINIMAL_STACK_SIZE),
        (void*)&BUTTON_TASK_DATA_ARRAY[0],
        (tskIDLE_PRIORITY + 1UL),
        &button_task_handle
    );
    configASSERT(ret == pdPASS);
}
