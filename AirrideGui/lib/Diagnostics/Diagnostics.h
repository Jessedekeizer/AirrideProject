#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

#include <Arduino.h>

#ifndef DIAG_HEAP
#define DIAG_HEAP 0
#endif

#if DIAG_HEAP

namespace Diagnostics {
    void ReportHeap(const char *when);

    void HeapTick();
}

#define DIAG_HEAP_REPORT(when) Diagnostics::ReportHeap(when)
#define DIAG_HEAP_TICK()       Diagnostics::HeapTick()

#else

#define DIAG_HEAP_REPORT(when) ((void)0)
#define DIAG_HEAP_TICK()       ((void)0)

#endif

#endif
