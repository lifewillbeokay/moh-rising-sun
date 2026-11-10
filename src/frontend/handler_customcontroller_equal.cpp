// HandlerCustomController::AreButtonsEqual; see docs/Frontend.md.
#include "PackageHandlers.h"
int HandlerCustomController::AreButtonsEqual(SFunctionNames *a, SFunctionNames *b) {
    if (a->type == b->type) {
        return buttons[a->button] == buttons[b->button];
    }
    if (a->type == EShellButtonStick || b->type == EShellButtonStick) {
        return 0;
    }
    int analog;
    int digital;
    if (a->type == EShellButtonAnalog) {
        analog = buttons[a->button];
        digital = buttons[b->button];
    } else {
        analog = buttons[b->button];
        digital = buttons[a->button];
    }
    if (analog == -1 || digital == -1) {
        return 0;
    }
    for (int i = 0; i < 9; i++) {
        if (analogEquivalence[i].analog == analog) {
            return analogEquivalence[i].digital == digital;
        }
    }
    return 0;
}
