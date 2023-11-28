#pragma once

#include <stdint.h>

typedef void (*dispatch_event_handler_t)(uint32_t, void*);

typedef struct
{
	uint32_t id;
	void* opt_data_address;
} Event;