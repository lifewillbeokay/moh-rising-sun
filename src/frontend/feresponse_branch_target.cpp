// AI-assisted GR8E69 reconstruction; see docs/research/feng-packages.md.
#ifdef DECOMP_COMPARE
#include "FEMessageResponse.h"
long FEMessageResponse::FindConditionBranchTarget(unsigned long index) const {
    if (index == count - 1) return count;
    long depth = 1;
    do {
        unsigned long id = responses[++index].id;
        if (id == 0x500) { if (depth == 1) depth = 0; }
        else if (id == 0x300 || id == 0x301) ++depth;
        else if (id == 0x501) --depth;
    } while (index < count && depth);
    return index;
}
#endif
