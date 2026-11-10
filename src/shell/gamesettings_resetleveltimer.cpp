// CGameSettings: the settings singleton and its saved data.
#include "GameSettingsViews.h"

void CGameSettings::ResetLevelTimer() { profile->saveData.stats.levelTimer = 0; }
void CGameSettings::UpdateLevelTimer(CPlayerObject *player, unsigned int elapsed) {
    if (((CPlayerTimerView *)player)->timerPaused) return;
    profile->saveData.stats.levelTimer += elapsed;
}
