// Runtime fault hooks for the uno_r4_wifi_debug environment. Everything here
// is compiled out unless DEBUG_HOOKS is defined (see platformio.ini).
//
// The hooks turn silent failures into a debugger stop at the point of failure:
// a task blowing its stack, the FreeRTOS heap running out, or a configASSERT
// firing all end up halted with a usable backtrace instead of a reset loop.
#ifdef DEBUG_HOOKS

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

#include "Debug.h"
#include "Logger.h"

// OpenOCD's FreeRTOS thread awareness looks up uxTopUsedPriority to size its
// ready-list scan. Kernel v10.5.1 already exports it (tasks.c), so nothing
// extra is needed here.

static void HaltOnFault() {
    Serial.flush();
    DEBUG_BREAK();
    taskDISABLE_INTERRUPTS();
    for (;;) {}
}

// configCHECK_FOR_STACK_OVERFLOW=2 calls this on every context switch out of a
// task whose stack watermark is gone. xTask is unusable at this point, the
// name is copied inside the TCB and still readable.
extern "C" void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    (void) xTask;
    LOG_ERROR("stack overflow in task", pcTaskName);
    HaltOnFault();
}

// configUSE_MALLOC_FAILED_HOOK=1 calls this when pvPortMalloc returns null,
// which on this board means the 8 kB FreeRTOS heap is exhausted (task stacks,
// queues and the CAN queues all come out of it).
extern "C" void vApplicationMallocFailedHook() {
    LOG_ERROR("FreeRTOS heap exhausted, free bytes:", xPortGetFreeHeapSize());
    HaltOnFault();
}

// configASSERT() routes through newlib's assert(), which would otherwise abort
// into a reset. Overriding it keeps the failing file/line/expression and stops
// in the debugger instead.
extern "C" void __assert_func(const char *file, int line, const char *func, const char *expr) {
    LOG_ERROR("assert failed:", expr, "at", file, line, "in", func ? func : "?");
    HaltOnFault();
}

#endif //DEBUG_HOOKS
