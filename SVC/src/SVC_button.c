// ------ inclusions ---------------------------------------------------
#include <stdio.h>
#include "cmsis_os.h"

#include "app_resources.h"

#include "SVC_button.h"

/// | Private typedef -----------------------------------------------------------
/// | Private define ------------------------------------------------------------
/// | Private macro -------------------------------------------------------------
/// | Private variables ---------------------------------------------------------
/// | Private function prototypes -----------------------------------------------

/// | Private functions ---------------------------------------------------------
void task_button(void* parameters)
{
	printf("[%s] Task Created\n", pcTaskGetName(NULL));
	while(1) {}
}
