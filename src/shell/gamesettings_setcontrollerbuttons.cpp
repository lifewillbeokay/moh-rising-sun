// CGameSettings: the settings singleton and its saved data.
#include "GameSettingsViews.h"

void CGameSettings::SetControllerButtons(int index, int set, short *buttons, int count) {
    if (set > 1) return;
    if (index > 3) return;
    if (count <= 13) return;
    for (int i = 0; i < 14; i++) profile->profiles[index].buttons[set][i] = buttons[i];
}
void CGameSettings::GetControllerButtons(int index, int set, short *buttons, int count) {
    if (set > 1) return;
    if (index > 3) return;
    if (count <= 13) return;
    for (int i = 0; i < 14; i++) buttons[i] = profile->profiles[index].buttons[set][i];
}
void CGameSettings::CopyNameIntoSettings(const char *name, int player) {
    strncpy(profile->profiles[profile->profileIndex[player]].name, name, 8);
}
char *CGameSettings::GetNameFromSettings(int player) { return profile->profiles[profile->profileIndex[player]].name; }
int CGameSettings::Get_PlayerControllerSetting(int player) {
    int index = GetProfileIndexFromPlayerID(player);
    return profile->profiles[index].controllerSetting;
}
