// CGameSettings: the settings singleton and its saved data.
#include "GameSettingsViews.h"

void CGameSettings::RestoreLevelAudio() {
    AmbientTrack_Select(profile->saveData.ambientTrack);
    MUSIC_SendEvent(profile->saveData.musicEvent);
}
void CGameSettings::RestoreCoreLevelStats() {
    SPlayerProfile *p = &profile->profiles[profile->profileIndex[0]];
    profile->saveData.stats = p->saves[p->currentSaveSlot].stats;
    profile->levelStats = profile->saveData.stats;
}
