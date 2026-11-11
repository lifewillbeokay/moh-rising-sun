// AI-assisted GR8E69 reconstruction; see docs/research/feng-packages.md.
#ifdef DECOMP_COMPARE
#include "FEPackageReader.h"
int FEPackageReader::ReadTypeSizes() {
    FEChunk *chunk = FindChild(root, 0x53707954);
    if (chunk) {
        types = (FETypeSize *)(chunk + 1);
        typeCount = FESwap(chunk->size) / sizeof(FETypeSize);
    }
    return 1;
}
#endif
