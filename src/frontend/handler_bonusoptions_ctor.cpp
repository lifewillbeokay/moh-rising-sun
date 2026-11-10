// HandlerBonusOptions, the bonus screen: construction; see docs/Frontend.md.
#include "PackageHandlers.h"
#include <stdio.h>
HandlerBonusOptions::HandlerBonusOptions() {
    char name[64];
    for (int i = 0; i < 12; i++) {
        sprintf(name, "bonus%.2d text", i + 1);
        textHashes[i] = FEHashUpper(name);
    }
}
