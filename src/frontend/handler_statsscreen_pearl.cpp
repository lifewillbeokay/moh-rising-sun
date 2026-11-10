// HandlerStatsScreen, the statistics screen: the special Pearl Harbor stats; see docs/Frontend.md.
#include "PackageHandlers.h"
#include "GameSettings.h"
#include "GameSettingsViews.h"
#include "PauseMenu.h"
#include <string.h>
#include <stdio.h>
void HandlerStatsScreen::CheckToSeeIfSpecialPearlStatsAreNeeded(bool useSelected) {
    specialPearlStats = 0;
    if (useSelected) {
        if (g_Shell.selectedLevel == 1 || g_Shell.selectedLevel == 11) specialPearlStats = 1;
    } else {
        SCoreGameSettings *core = g_GameSettings.profile;
        if (core->saveData.mission == 1 || core->saveData.mission == 5) {
            int level = core->saveData.level;
            if (level == 1 || level == 11) specialPearlStats = 1;
        }
    }
}
