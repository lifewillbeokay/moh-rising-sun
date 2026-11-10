// CGameSettings: the settings singleton and its saved data.
#include "GameSettingsViews.h"

int CGameSettings::GetProfileIndexFromPlayerID(int player) {
    if (g_bCoopMultiplayer || g_Shell.inCoop) return profile->profileIndex[player];
    if (g_bInMultiplayerMode || g_Shell.inMultiplayer) return profile->profileIndex[player];
    return 0;
}
int CGameSettings::GetPlayerIDFromProfileIndex(int index) {
    if (g_bCoopMultiplayer || g_Shell.inCoop || g_bInMultiplayerMode || g_Shell.inMultiplayer) {
        for (int i = 0; i < 4; i++) if (profile->profileIndex[i] == index) return i;
    }
    return 0;
}
int CGameSettings::WasLastGameSinglePlayer() { return profile->wasLastGameSinglePlayer; }
SPlayerProfile *CGameSettings::GetProfile(unsigned int player) { return &profile->profiles[profile->profileIndex[player]]; }
