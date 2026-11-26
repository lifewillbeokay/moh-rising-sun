// AI-assisted GR8E69 reconstruction; see docs/research/save-popups.md.
#include "SaveGamePopups.h"
#include <stdlib.h>
extern CSaveGameLanguageScreen languageScreen asm("p.3348");
extern int languageScreenGuard asm("_.tmp_0.3349");
// Explicit original constructor boundary, for the original static object's storage.
extern "C" void ConstructLanguageScreen(CSaveGameLanguageScreen *) asm("__23CSaveGameLanguageScreen");
static void DestroyLanguageScreen() asm("__tcf_0");
static void DestroyLanguageScreen() { languageScreen.CSaveGameLanguageScreen::~CSaveGameLanguageScreen(); }
CSaveGameLanguageScreen *CSaveGameLanguageScreen::Instance() {
    if (!languageScreenGuard) {
        ConstructLanguageScreen(&languageScreen); languageScreenGuard = 1; atexit(DestroyLanguageScreen);
    }
    return &languageScreen;
}
