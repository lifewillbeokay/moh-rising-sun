// FEPackageReader: chunk readers; see docs/Frontend.md.
#include "FEPackageReader.h"

unsigned long FEPackageReader::GetTypeSize(unsigned long type) {
    for (unsigned long i = 0; i < typeCount; i++) {
        if (FESwap(types[i].type) == type) return FESwap(types[i].size);
    }
    return 0;
}
