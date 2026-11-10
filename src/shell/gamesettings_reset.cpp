// CGameSettings: the settings singleton and its saved data.
#include "GameSettingsViews.h"

void SPlayerStats::Reset() {
    dirty = 0;
    enemiesKilled = 0;
    hits = 0;
    hitsTaken = 0;
    unknown_a = 0;
    objectives = 0;
    bonusObjectives = 0;
    shotLocation[0] = 0;
    shotLocation[3] = 0;
    shotLocation[6] = 0;
    shotLocation[4] = 0;
    shotLocation[1] = 0;
    shotLocation[5] = 0;
    shotLocation[2] = 0;
    for (int i = 0; i < 8; i++) {
        entries[i].value = 0;
        entries[i].count = 0;
    }
    shotsFired = 0;
    levelTimer = 0;
}
SSaveData::SSaveData()
    : level(0), mission(0), ambientTrack(-12), savePointID(0), levelWon(0), unknownBits(0), musicEvent(-1), oneShotBank(-1), unknown_144(0) {
    for (unsigned int i = 0; i < 4; i++) compartmentID[i] = i;
    TotalReset();
}
void SSaveData::TotalReset() {
    mission = 0;
    level = 0;
    LevelReset();
}
// SCRC_Ammo::Reset's statements, inlined here but called out of line by the constructor.
static inline void ClearAmmo(SCRC_Ammo &a) {
    a.weaponId = 0;
    a.ammoA = 0;
    a.selectable = 0;
    a.ammoB = 0;
}
void SSaveData::LevelReset() {
    savePointID = 0;
    levelWon = 0;
    unknownBits = 0;
    ambientTrack = -12;
    oneShotBank = -1;
    musicEvent = -1;
    for (unsigned int player = 0; player < 2; player++) {
        currentWeapon[player] = 0;
        health[player] = 0.0f;
        for (int i = 0; i < 8; i++) ClearAmmo(ammo[player][i]);
    }
    stats.Reset();
}
