#pragma once

#include "ao.h"

/// @brief Initialize the SYS Active Object.
/// @param ao Active Object to initialize
/// @param ao_task_name Name for the task
bool svc_sys_initialize(ActiveObject* const ao, const char* ao_task_name);
