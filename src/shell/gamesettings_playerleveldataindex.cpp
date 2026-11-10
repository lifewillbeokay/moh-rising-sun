// CGameSettings: statistics and level records.
#include "GameSettingsViews.h"
#include "Objectives.h"
#include "Toilet.h"
void iDebugMsg(const char *, ...);
void DebugMsg(const char *, ...);
extern int g_bMPEndLevel;

int CGameSettings::PlayerLevelDataIndex(int mission, int level) {
    SPlayerLevelData *data = profile->profiles[profile->profileIndex[0]].levelData;
    for (int i = 0; i < 22; i++, data++) {
        if (data->mission == mission && data->level == level) return i;
    }
    return -1;
}
