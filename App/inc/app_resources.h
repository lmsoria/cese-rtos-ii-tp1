#pragma once

/// | Includes ------------------------------------------------------------------

#include <FreeRTOS.h>
#include <task.h>

#include "ao.h"
#include "dyn_ao.h"

#include "HAL_button.h"

/// | Exported types ------------------------------------------------------------

/// @brief Struct that represents the data consumed by the button service
typedef struct
{
    BoardButtons button;
} ButtonTaskData;

/// | Exported data -------------------------------------------------------------

extern TaskHandle_t button_task_handle;
extern DynamicAO ao_led;
extern ActiveObject ao_sys;

/// | Exported constants --------------------------------------------------------
/// | Exported macro ------------------------------------------------------------
/// | Exported functions --------------------------------------------------------
