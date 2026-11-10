// CSaveGameLanguageScreen's constructor; see docs/Frontend.md.
#include "SaveGamePopups.h"
extern void *g_pMiscTextDatabase;
extern "C" void LOCALE_setstate(void *, int, int);
CSaveGameLanguageScreen::CSaveGameLanguageScreen() : CSaveGamePopup("LanguageSelect.fng") {
    for (int i = 0; i < 5; i++) {
        char name[40];
        sprintf(name, "Language Button %d", i);
        buttonHashes[i] = FEHashUpper(name);
    }
    languages[0] = 0;
    languages[1] = 2;
    languages[2] = 3;
    languages[3] = 6;
    languages[4] = 7;
    LOCALE_setstate(g_pMiscTextDatabase, 0, 0);
    idleFrames = 0;
}
CSaveGameLanguageScreen::~CSaveGameLanguageScreen() {}
