// CGameSettings: the settings singleton and its saved data.
#include "GameSettingsViews.h"

void CGameSettings::InitHUDFromOptions(int player) {
    CPlayerHudView *view = (CPlayerHudView *)g_scene.GetPlayer(player);
    view->hudOption = g_GameSettings.profile->profiles[player].hudOption;
}
void CGameSettings::InitAudioFromOptions(int player, bool) {
    SoundSetVolumes(g_GameSettings.profile->profiles[g_GameSettings.profile->profileIndex[0]].volumeA,
                    g_GameSettings.profile->profiles[g_GameSettings.profile->profileIndex[0]].volumeB);
}
