#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

//Problema 1
//
//Diseñar e implementar un módulo para las creación/inicializacion de objetos activos: creación de task y queue, inicializacion de estructura.
//La idea sería contar con una función con en siguiente prototipo:
//
//bool InicializarObjetoActivo(objact_t*, void (*funcion_handle)(void));
//
//El primer argumento es la estructura del objeto activo que queremos Inicializar, en segundo el puntero a función del proceso que hará el objeto activo.
//Retorna true so lo puede iniciar, false sino.

//Con base en el problema anterior, se pide modificarlo para poder pasar argumentos a la función handle del objeto activo. El prototipo final podría ser de la siguiente forma.
//
//bool InicializarObjetoActivo(objact_t*, void (*funcion_handle)(void), void*);

typedef void (*dispatch_event_handler_t)(uint32_t, void*);

typedef struct
{
	uint32_t id;
	void* opt_data_address;
} Event;

typedef struct
{
    QueueHandle_t queue;
    TaskHandle_t* task;
    dispatch_event_handler_t dispatch_function;
} ActiveObject;

bool ao_initialize(ActiveObject* const ao, const char* task_name, dispatch_event_handler_t handler);
