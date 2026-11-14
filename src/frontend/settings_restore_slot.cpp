// AI-assisted reconstruction from GR8E69; see docs/research/frontend-popup-list.md.
#include "GameSettings.h"
#ifdef DECOMP_COMPARE
void CGameSettings::RestoreSaveData(unsigned int slot, unsigned int player) {
    SCoreGameSettings *savedProfile = profile;
    slotBackupSlot = slot;
    slotBackupPlayer = player;
    levelStartData = GetProfile(player)->saves[slot];
    savedProfile->saveData = levelStartData;
    profile->levelStats = profile->saveData.stats;
    GetProfile(player)->currentSaveSlot = slot;
}
#endif
