#pragma once

#include "stdint.h"

uint32_t* initialize_stack(void (*fn_ptr)(void*), int stack_size, void* arguments);
void thread_handler(void* (fn_ptr(void*)));
