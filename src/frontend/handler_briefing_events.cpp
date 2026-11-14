// AI-assisted GR8E69 reconstruction; see docs/research/frontend-screens.md.
#ifdef DECOMP_COMPARE
#include "PackageHandlers.h"
#include "GameSettings.h"
#include <stdio.h>
extern int g_bCoopMultiplayer;
extern "C" int strcasecmp(const char *, const char *);
int HandlerPauseBriefingOverlay::HandleEvent(unsigned int event, FEObject *, unsigned long) {
    unsigned int language = GetCurrentLanguage();
    if (event == FEHashUpper("Setup Menu")) {
        const char *screen = package->name;
        if (!strcasecmp(screen, "BriefingOverlay.fng")) isHistory = 0;
        else if (!strcasecmp(screen, "HistoryOverlay.fng")) isHistory = 1;
        unsigned char level = g_GameSettings.profile->saveData.level;
        for (int i = 1; i < 13; ++i) {
            char label[32], name[32];
            if (!isHistory) sprintf(label, g_bCoopMultiplayer ? "BRIEFING_TEXT_COOP%d_%d" : "BRIEFING_TEXT%d_%d", i, level);
            else sprintf(label, "HISTORY_TEXT%d_%d", i, level);
            sprintf(name, "text %d", i);
            FEWideString *localized = g_Shell.localizer->GetString(label, language);
            FEStringObject *object;
            if (!localized) { object = (FEStringObject *)package->FindObjectByName(name); object->text = ""; }
            else { object = (FEStringObject *)package->FindObjectByName(name); object->text = localized->text; }
            object->flags |= 0x400000;
        }
    }
    return 0;
}
#endif
