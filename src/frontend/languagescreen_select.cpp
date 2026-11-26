// AI-assisted GR8E69 reconstruction; see docs/research/save-popups.md.
#include "SaveGamePopups.h"
extern int g_eResponse;
#ifdef DECOMP_COMPARE
void CSaveGameLanguageScreen::SelectLanguage() {
    Push(); g_eResponse = 0;
    do {
        if (++idleFrames >= 900) { g_Shell.PlayMovie(&g_Shell.movies.demo, true); idleFrames = 0; }
        Wait();
    } while (!g_eResponse);
    Pop();
}
#endif
