// CGameSettings: the settings singleton and its saved data.
#include "GameSettingsViews.h"

int CGameSettings::HasBeenUnlocked(int level) const {
    int mask = 1 << level;
    int unlocked;
    if (IsCoop()) unlocked = profile->profiles[profile->profileIndex[0]].coopUnlockedLevels;
    else unlocked = profile->profiles[profile->profileIndex[0]].unlockedLevels;
    if (unlocked & mask) return 1;
    return 0;
}
void CGameSettings::AddCheat(int cheat) {
    if (IsCoop()) {
        int mask = 1 << cheat;
        profile->profiles[profile->profileIndex[0]].coopCheats |= mask;
        profile->profiles[profile->profileIndex[1]].coopCheats |= mask;
    } else profile->profiles[profile->profileIndex[0]].cheats |= 1 << cheat;
}
void CGameSettings::UnlockLevel(int level, bool invert) {
    int coop = 0;
    if (IsCoop()) coop = 1;
    if (invert) coop ^= 1;
    if (coop) {
        profile->profiles[profile->profileIndex[0]].coopUnlockedLevels |= 1 << level;
        profile->profiles[profile->profileIndex[1]].coopUnlockedLevels |= profile->profiles[profile->profileIndex[0]].coopUnlockedLevels;
    } else profile->profiles[profile->profileIndex[0]].unlockedLevels |= 1 << level;
}
