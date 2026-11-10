// CGameSettings: the settings singleton and its saved data.
#include "GameSettingsViews.h"

int CGameSettings::GetIfCurrentWeaponSelectable(int weaponId, int player) const {
    for (int i = 0; i < 8; i++) {
        if (profile->saveData.ammo[player][i].weaponId == weaponId) {
            if (profile->saveData.ammo[player][i].selectable) return 1;
            return 0;
        }
    }
    return 0;
}
int CGameSettings::GetPlayersFinishedLevelSuccessfully(int mission, int level) {
    int index = PlayerLevelDataIndex(mission, level);
    return profile->profiles[profile->profileIndex[0]].levelData[index].finished ? 1 : 0;
}
