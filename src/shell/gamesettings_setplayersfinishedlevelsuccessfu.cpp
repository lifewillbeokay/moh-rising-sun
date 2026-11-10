// CGameSettings: the settings singleton and its saved data.
#include "GameSettingsViews.h"

void CGameSettings::SetPlayersFinishedLevelSuccessfully(int mission, int level) {
    int index = PlayerLevelDataIndex(mission, level);
    profile->profiles[profile->profileIndex[0]].levelData[index].finished = 1;
}
void CGameSettings::SetPlayersFinishedLevelHealth(int mission, int level, float health) {
    int index = PlayerLevelDataIndex(mission, level);
    profile->profiles[profile->profileIndex[0]].levelData[index].health = health;
}
void CGameSettings::SetCurrentHealth(float health, int player) { profile->saveData.health[player] = health; }
float CGameSettings::GetCurrentHealth(int player) const { return profile->saveData.health[player]; }
void CGameSettings::SetCurrentAmmo(int weaponId, unsigned int a, unsigned int b, int player) {
    for (int i = 0; i < 8; i++) {
        int id = profile->saveData.ammo[player][i].weaponId;
        if (id == weaponId || id == 0) {
            profile->saveData.ammo[player][i].weaponId = weaponId;
            profile->saveData.ammo[player][i].ammoA = a;
            profile->saveData.ammo[player][i].ammoB = b;
            return;
        }
    }
}
void CGameSettings::SetCurrentWeapon(CWeapon *weapon, int player) {
    int id = 0;
    if (weapon) id = *(int *)((char *)weapon + 776);
    profile->saveData.currentWeapon[player] = id;
}
int CGameSettings::GetCurrentWeapon(int &id, int player) {
    id = profile->saveData.currentWeapon[player];
    if (id) return 1;
    return 0;
}
void CGameSettings::GetCurrentAmmo(int weaponId, unsigned int &a, unsigned int &b, int player) const {
    b = 0;
    a = 0;
    for (int i = 0; i < 8; i++) {
        if (profile->saveData.ammo[player][i].weaponId == weaponId) {
            a = profile->saveData.ammo[player][i].ammoA;
            b = profile->saveData.ammo[player][i].ammoB;
            return;
        }
    }
}
