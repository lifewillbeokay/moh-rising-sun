// AI-assisted GR8E69 reconstruction; see docs/research/feng-packages.md.
#ifdef DECOMP_COMPARE
#include "FEEvent.h"
#include "FEngStrings.h"
#include <string.h>
void FEEventList::SetCount(long newCount) {
    if (newCount == count) return;
    if (!newCount) {
        // The zero-count path uses scalar delete in the original.
        if (events) delete events;
        count = 0; events = 0;
        return;
    }
    FEEvent *replacement = new ("source/FEngine/FEEvent.cpp:161") FEEvent[newCount];
    if (newCount < count) memcpy(replacement, events, newCount * sizeof(FEEvent));
    else {
        memcpy(replacement, events, count * sizeof(FEEvent));
        memset(replacement + count, 0, (newCount - count) * sizeof(FEEvent));
    }
    if (events) delete[] events;
    count = newCount; events = replacement;
}
#endif
