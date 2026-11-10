// FEngine's list and marker helpers; see docs/Frontend.md.
#include "FEngine.h"
#include <string.h>

void FEngine::RecordLastPackageButton(unsigned long package, unsigned long button) {
    for (int i = 0; i < 32; i++) {
        if (lastButtons[i].package == package) lastButtons[i].package = 0;
    }
    lastButtons[lastButtonIndex].package = package;
    lastButtons[lastButtonIndex].button = button;
    lastButtonIndex = (lastButtonIndex + 1) % 32;
}
unsigned long FEngine::RecallLastPackageButton(unsigned long package) {
    for (int i = 0; i < 32; i++) {
        if (lastButtons[i].package == package) return lastButtons[i].button;
    }
    return 0;
}
