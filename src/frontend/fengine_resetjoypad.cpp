// FEngine's list and marker helpers; see docs/Frontend.md.
#include "FEngine.h"
#include <string.h>

void FEngine::ResetJoyPad() {
    int zero = 0;
    joyPadCount = zero;
    memset(joyPadState, 0, 76);
    activeMask = zero;
}
unsigned long FEngine::GetCombinedJoyPadActiveMask() const {
    unsigned long mask = activeMask;
    for (FEPackage *p = (FEPackage *)packages.head; p; p = (FEPackage *)p->next) {
        unsigned short control = p->controlMask;
        if (control) mask &= control;
    }
    if (!mask) mask = activeMask;
    return mask;
}
void FEngine::SetExecution(bool execute) {
    executing = execute;
    for (FEPackage *p = (FEPackage *)packages.head; p; p = (FEPackage *)p->next) p->executing = executing;
}
