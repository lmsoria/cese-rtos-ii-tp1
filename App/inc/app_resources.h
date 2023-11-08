#pragma once

/// | Includes ------------------------------------------------------------------

#include <FreeRTOS.h>
#include <task.h>

#include "HAL_button.h"
#include "SVC_led.h"

/// | Exported types ------------------------------------------------------------

/// @brief Struct that represents the data consumed by the button service
typedef struct
{
	BoardButtons button;
} ButtonTaskData;

/// | Exported data -------------------------------------------------------------

extern TaskHandle_t button_task_handle;
extern LEDActiveObject led_ao_green;

/// | Exported constants --------------------------------------------------------
/// | Exported macro ------------------------------------------------------------
/// | Exported functions --------------------------------------------------------
