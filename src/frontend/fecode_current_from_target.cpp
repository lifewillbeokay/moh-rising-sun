// AI-assisted reconstruction from GR8E69; see docs/research/feng-animation-lists.md.
#ifdef DECOMP_COMPARE
#include "FEngElements.h"
#include "FEListIndex.h"

unsigned long FECodeListBox::CalculateCurrentFromTarget(unsigned long target, unsigned long total, unsigned long visible) {
    if (target >= total) target = total ? total - 1 : 0;
    if (listFlags & 8) return FEWrapIndex(target - (visible >> 1), total);
    return target;
}

#endif
