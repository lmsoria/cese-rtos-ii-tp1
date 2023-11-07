#include <stdio.h>

#include "cmsis_os.h"

#include "app.h"
#include "app_resources.h"



#include "SVC_led.h"

/// | Private typedef -----------------------------------------------------------
/// | Private define ------------------------------------------------------------
/// | Private macro -------------------------------------------------------------
/// | Private function prototypes -----------------------------------------------
/// | Private variables ---------------------------------------------------------
static LEDActiveObject led_ao_green;
/// | Exported variables -------------------------------------------------------

/// | Private functions ---------------------------------------------------------

void app_init()
{
    printf("Main application starts here\n");

    led_initialize_ao(&led_ao_green, "ao_led_green");


}
