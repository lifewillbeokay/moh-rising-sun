// HandlerCustomController's key table lookups; see docs/Frontend.md.
#include "PackageHandlers.h"
int HandlerCustomController::FindKeyIndex(short key, EShellButtonType type) {
    switch (type) {
    case EShellButtonDigital:
        for (int i = 0; i < 9; i++) {
            if (controllerKeyNames[i].key == key) {
                return i;
            }
        }
        break;
    case EShellButtonAnalog:
        for (int i = 0; i < 9; i++) {
            if (analogKeyNames[i].key == key) {
                return i;
            }
        }
        break;
    case EShellButtonStick:
        for (int i = 0; i < 4; i++) {
            if (analogStickNames[i].key == key) {
                return i;
            }
        }
        break;
    }
    return 0;
}
int HandlerCustomController::FindButtonIndex(unsigned int hash) {
    int count = 10;
    for (int i = 0; i < count; i++) {
        if (hash == FEHashUpper(controllerFunctionNames[i].name)) {
            return i;
        }
    }
    return -1;
}
int HandlerCustomController::GetNumKeyNames(EShellButtonType type) {
    int count = 0;
    switch (type) {
    case EShellButtonDigital:
        count = 9;
        break;
    case EShellButtonAnalog:
        count = 9;
        break;
    case EShellButtonStick:
        count = 4;
        break;
    }
    return count;
}
int HandlerCustomController::GetNumAssignableFunctions() { return 10; }
int HandlerCustomController::AssignToNextKeyIndex(int function, int step) {
    if (step > 0) {
        g_Shell.PlayShellSound((EShellSoundType)6);
    } else {
        g_Shell.PlayShellSound((EShellSoundType)7);
    }
    int index = controllerFunctionNames[function].keyIndex + step;
    int count = GetNumKeyNames(controllerFunctionNames[function].type);
    if (index < 0) {
        index = count - 1;
    } else if (index >= count) {
        index = 0;
    }
    return index;
}
