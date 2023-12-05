/// | Includes ------------------------------------------------------------------
#include <stdio.h>

#include "app.h"

#include "SVC_button.h"
#include "SVC_led.h"
#include "SVC_sys.h"

/// | Private typedef -----------------------------------------------------------
/// | Private define ------------------------------------------------------------
/// | Private macro -------------------------------------------------------------
/// | Private function prototypes -----------------------------------------------
/// | Private variables ---------------------------------------------------------
/// | Exported variables -------------------------------------------------------
/// | Exported variables --------------------------------------------------------
/// | Private functions ---------------------------------------------------------

void app_init()
{
    printf("Main application starts here\n");

    // Initialize LED Service
    if(!svc_led_initialize()) {
    	printf("Error: Couldn't initialize led service\n");
    	while(1);
    }

    // Initialize SYS Service
    if(!svc_sys_initialize()) {
    	printf("Error: Couldn't initialize sys service\n");
    	while(1);
    }

    // Initialize button service
    if(!svc_button_initialize()) {
    	printf("Error: Couldn't initialize button service\n");
    	while(1);
    }
}
