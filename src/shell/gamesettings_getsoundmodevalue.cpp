// CGameSettings: the settings singleton and its saved data.
#include "GameSettingsViews.h"

int CGameSettings::GetSoundModeValue(int player) {
    if (OSGetSoundMode() == 0) return 0;
    int mode = profile->profiles[profile->profileIndex[player]].soundMode;
    switch (mode) {
    case 0: mode = 1; break;
    case 1: return 2;
    case 2: mode = 1; break;
    default: mode = 1; break;
    }
    return mode;
}
void CGameSettings::SetSoundModeValue(int player, ESoundMode mode) {
    switch (mode) {
    case 1: profile->profiles[profile->profileIndex[player]].soundMode = 0; break;
    case 2: profile->profiles[profile->profileIndex[player]].soundMode = 1; break;
    case 0: profile->profiles[profile->profileIndex[player]].soundMode = 2; break;
    default: profile->profiles[profile->profileIndex[player]].soundMode = 0; break;
    }
    if (mode == 0) OSSetSoundMode(0);
    else OSSetSoundMode(1);
}
int CGameSettings::CurrentLevelIsShell() const {
    int mission = profile->saveData.mission;
    int level = profile->saveData.level;
    return mission == 1 && level == 16;
}
void CGameSettings::BindKeyToEvent(KEYMAP *map, unsigned short key, unsigned short event) {
    for (int i = 0; map[i].key != 0xffff; i++) {
        if (map[i].key == key) {
            map[i].event = event;
            return;
        }
    }
}
