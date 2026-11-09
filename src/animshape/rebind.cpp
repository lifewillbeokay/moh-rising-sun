// AI-assisted reconstruction from GR8E69; see docs/research/broad-source-sweep.md.
#include "AnimShape.h"

#include "TARExtension.h"
#ifdef DECOMP_COMPARE
void AnimShape::RebindTARs() {
    if (dirty) {
        SHAPE *old = boundShape;
        boundShape = pendingShape;
        pendingShape = old;
        dirty = 0;
        for (EAGL::TAR **tar = tars.begin; tar != tars.end; ++tar)
            (*tar)->SwapShape(boundShape);
    }
}
#endif
