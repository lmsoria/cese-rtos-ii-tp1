#pragma once

/// @brief Events to be detected by the button task
typedef enum
{
    BUTTON_EVENT_INITIAL, ///< Initial state.
    BUTTON_EVENT_SHORT,   ///< Detected when the button is being pressed in the range [EVENT_SHORT_THRESHOLD_MIN_MS, EVENT_LONG_THRESHOLD_MIN_MS)
    BUTTON_EVENT_LONG,    ///< Detected when the button is being pressed in the range [EVENT_LONG_THRESHOLD_MIN_MS, EVENT_BLOCKED_THRESHOLD_MIN_MS)
    BUTTON_EVENT_BLOCKED, ///< Detected when the button is being pressed in the range >= EVENT_BLOCKED_THRESHOLD_MIN_MS,
	BUTTON_EVENT_RELEASED,///< Detected when the button is being released.
} ButtonEvent;

/// @brief Initialize the button service.
/// @return true if the service was initialized successfully. False otherwise.
bool svc_button_initialize();
