#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>

// DEBUG_BREAK() halts the CPU in GDB when a debugger is attached and is a
// no-op otherwise, so it is safe to leave in code that also builds for the
// non-debug environments. Only the uno_r4_wifi_debug env defines DEBUG_HOOKS.
#ifdef DEBUG_HOOKS

inline bool DebuggerAttached() {
    return (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) != 0;
}

#define DEBUG_BREAK()                     \
    do {                                  \
        if (DebuggerAttached()) {         \
            __BKPT(0);                    \
        }                                 \
    } while (0)

#else

inline bool DebuggerAttached() { return false; }

#define DEBUG_BREAK() do { } while (0)

#endif

#endif //DEBUG_H
