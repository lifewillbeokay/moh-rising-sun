// AI-assisted reconstruction from GR8E69; see docs/research/feng-animation-lists.md.
#ifdef DECOMP_COMPARE
#include "FEngElements.h"
#include "FEListIndex.h"
#include "ShellManager.h"

int FECodeListBox::CheckMovement(long step, long current, long target, long total, long visible) {
    if ((listFlags & 4) && visible >= total) {
        game->NotificationMessage(FEHashUpper("CLB AT MIN"), this, 255, 0);
        game->NotificationMessage(FEHashUpper("CLB AT MAX"), this, 255, 0);
        return 0;
    }
    if (listFlags & 2) {
        long position = (listFlags & 4) ? current + step : target + step;
        if (position < 0) {
            game->NotificationMessage(FEHashUpper("CLB AT MIN"), this, 255, 0);
            return 0;
        }
        if (position >= ((listFlags & 4) ? total - visible : total)) {
            game->NotificationMessage(FEHashUpper("CLB AT MAX"), this, 255, 0);
            return 0;
        }
    }
    return 1;
}

int FECodeListBox::MakeMove(long step, unsigned long &current, unsigned long &target,
                           unsigned long total, unsigned long visible) {
    if (listFlags & 8) {
        current = FEWrapIndex(current + step, total);
        target = FEWrapIndex(target + step, total);
        return 1;
    }
    if ((listFlags & 6) == 6) {
        current = FEWrapIndex(current + step, total);
        target = current;
        return 1;
    }
    unsigned long oldTarget = target;
    target = FEWrapIndex(target + step, total);
    if (step < 0) {
        if (current != oldTarget) return 0;
        current = target;
    } else {
        if (current == oldTarget) return 0;
        unsigned long distance = current < target ? target - current : target + total - current;
        if (distance < visible) return 0;
        current = FEWrapIndex(current + step, total);
    }
    return 1;
}

#endif
