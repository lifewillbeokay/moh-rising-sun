// CGameSettings: the settings singleton and its saved data.
#include "GameSettingsViews.h"

int CGameSettings::HasMedal(int medal) const {
    if (IsCoop()) return (profile->profiles[profile->profileIndex[0]].coopMedals >> medal) & 1;
    return (profile->profiles[profile->profileIndex[0]].medals >> medal) & 1;
}
int CGameSettings::HasCheat(int cheat) const {
    if (IsCoop()) return 0;
    return (profile->profiles[profile->profileIndex[0]].cheats >> cheat) & 1;
}
int CGameSettings::IsCheatActive(int cheat) const {
    if (IsCoop()) return 0;
    return (profile->profiles[profile->profileIndex[0]].activeCheats >> cheat) & 1;
}
void CGameSettings::ActivateCheat(int cheat, bool active) {
    if (IsCoop()) return;
    if (active) profile->profiles[profile->profileIndex[0]].activeCheats |= 1 << cheat;
    else profile->profiles[profile->profileIndex[0]].activeCheats &= ~(1 << cheat);
}
int CGameSettings::Get_currentDifficulty() const {
    if (IsCoop()) return profile->profiles[profile->profileIndex[0]].coopDifficulty;
    return profile->profiles[profile->profileIndex[0]].difficulty;
}
