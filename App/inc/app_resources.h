#pragma once

/// | Includes ------------------------------------------------------------------

#include "HAL_button.h"

/// | Exported types ------------------------------------------------------------

/// @brief Struct that represents the data consumed by the button service
typedef struct
{
	BoardButtons button;
} ButtonTaskData;


/// | Exported data -------------------------------------------------------------

extern TaskHandle_t button_task_handle;

/// | Exported constants --------------------------------------------------------
/// | Exported macro ------------------------------------------------------------
/// | Exported functions --------------------------------------------------------
