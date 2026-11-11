// AI-assisted GR8E69 reconstruction; see docs/research/feng-packages.md.
#ifdef DECOMP_COMPARE
#include "FEPackageReader.h"
FEChunk *FEPackageReader::FindChild(FEChunk *parent, unsigned long id) {
    FEChunk *end = (FEChunk *)((char *)(parent + 1) + FESwap(parent->size));
    FEChunk *child = parent + 1;
    // The target reads the sentinel header at end as well as the children.
    while (FESwap(child->id) != id && child != end)
        child = (FEChunk *)((char *)(child + 1) + FESwap(child->size));
    return FESwap(child->id) == id ? child : 0;
}
#endif
