#pragma once

/// | Includes ------------------------------------------------------------------

#include <FreeRTOS.h>
#include <task.h>

#include "ao.h"

#include "HAL_button.h"

/// | Exported types ------------------------------------------------------------

/// @brief Struct that represents the data consumed by the button service
typedef struct
{
    BoardButtons button;
} ButtonTaskData;

/// | Exported data -------------------------------------------------------------

extern TaskHandle_t button_task_handle;
extern ActiveObject ao_led;

/// | Exported constants --------------------------------------------------------
/// | Exported macro ------------------------------------------------------------
/// | Exported functions --------------------------------------------------------
