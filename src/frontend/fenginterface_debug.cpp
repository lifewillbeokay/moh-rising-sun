// CFengInterface's debug prints; see docs/Frontend.md.
#include "FengInterface.h"
void CFengInterface::DebugMessageQueued(unsigned long message, FEObject *from, FEPackage *, FEObject *to, unsigned long mask) {
    printf("FEng Queued msg %X, from %X to %X, mask is %X\n", message, from, to, mask);
}
void CFengInterface::DebugMessageProcessed(unsigned long message, FEObject *from, FEObject *to, FEPackage *, unsigned long mask) {
    printf("FEng Processed msg %X, from %X to %X, mask is %X\n", message, to, from, mask);
}
