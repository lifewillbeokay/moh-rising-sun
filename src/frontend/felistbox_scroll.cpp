// AI-assisted reconstruction from GR8E69; see docs/research/feng-animation-lists.md.
#ifdef DECOMP_COMPARE
#include "FEListBox.h"
#include <math.h>

// One axis of the same cumulative-distance walk; helper name is descriptive.
static inline void ScrollAxis(long step, unsigned long &current, unsigned long count,
                              FEListEntryData *entries, float extent, float &position,
                              float &target, unsigned long &flags, unsigned long edgeFlag,
                              unsigned long activeFlag) {
    unsigned long previous = current;
    unsigned long next = previous + step;
    if (flags & 4) {
        if ((long)next >= (long)count) next = count - 1;
        if ((long)next < 0) next = 0;
        current = next;
        target = entries[next].cumulative;
    } else if ((long)next < 0) {
        unsigned long at = count + next;
        position = target = entries[at].cumulative;
        do {
            position += entries[at].size;
            at = (at + 1) % count;
        } while (at != previous);
        flags |= 8;
        current = next % count;
    } else if ((long)next >= (long)count) {
        target = position;
        do {
            target += entries[previous % count].size;
            ++previous;
        } while (previous != next);
        flags |= 8;
        current = next % count;
    } else {
        current = next;
        target = entries[next].cumulative;
    }
    float remaining = 0.0f;
    for (unsigned long i = current; i < count; ++i) remaining += entries[i].size;
    if (flags & 4) {
        if (!(remaining >= extent)) target -= extent - remaining;
    } else if (!(remaining >= extent)) flags |= edgeFlag;
    flags |= activeFlag;
}

void FEListBox::ScrollSelection(long column, long row) {
    if ((listFlags & 0x60) == 0x60) return;
    if (listFlags & 0x20) column = 0;
    if (listFlags & 0x40) row = 0;
    if (!column && !row) return;
    if (column && !(listFlags & 0x20))
        ScrollAxis(column, currentColumn, numColumns, columns, unknown_64, scrollX, targetX, listFlags, 8, 0x20);
    if (row && !(listFlags & 0x40))
        ScrollAxis(row, currentRow, numRows, rows, unknown_68, scrollY, targetY, listFlags, 16, 0x40);
    listFlags |= 2;
    unknown_98 = targetX - scrollX;
    unknown_9c = targetY - scrollY;
    if (!(sqrtf(unknown_98 * unknown_98 + unknown_9c * unknown_9c) >= 0.1f)) {
        CompleteScroll();
    } else {
        float inverse = 1.0f / sqrtf(unknown_98 * unknown_98 + unknown_9c * unknown_9c);
        unknown_98 *= inverse;
        unknown_9c *= inverse;
    }
}

#endif
