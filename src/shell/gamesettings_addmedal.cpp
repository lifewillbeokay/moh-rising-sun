// CGameSettings: statistics and level records.
#include "GameSettingsViews.h"
#include "Objectives.h"
#include "Toilet.h"
void iDebugMsg(const char *, ...);
void DebugMsg(const char *, ...);
extern int g_bMPEndLevel;

void CGameSettings::AddMedal(int medal) {
    if (IsCoop()) {
        int mask = 1 << medal;
        profile->profiles[profile->profileIndex[0]].coopMedals |= mask;
        if (((profile->profiles[profile->profileIndex[0]].coopMedals ^ 3006) & 3006) == 0) {
            AddCheat(31);
            profile->profiles[profile->profileIndex[0]].coopMedals |= 1024;
        }
        profile->profiles[profile->profileIndex[1]].coopMedals |= mask;
        if (((profile->profiles[profile->profileIndex[1]].coopMedals ^ 3006) & 3006) == 0) {
            AddCheat(31);
            profile->profiles[profile->profileIndex[1]].coopMedals |= 1024;
        }
    } else {
        profile->profiles[profile->profileIndex[0]].medals |= 1 << medal;
        if (((profile->profiles[profile->profileIndex[0]].medals ^ 3006) & 3006) == 0) {
            AddCheat(31);
            profile->profiles[profile->profileIndex[0]].medals |= 1024;
        }
    }
}
