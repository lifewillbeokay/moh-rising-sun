// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
#include "GameSettings.h"
#include "ScriptRuntime.h"
void CPlayerObject::SetFinishedLevelData() {
    SCoreGameSettings *profile = g_GameSettings.profile;
    if (profile->saveData.levelWon) {
        int mission = profile->saveData.mission;
        int level = profile->saveData.level;
        g_GameSettings.SetPlayersFinishedLevelSuccessfully(mission, level);
        g_GameSettings.SetPlayersFinishedLevelHealth(mission, level, *(float *)script->gameObject->GetScriptData());
    }
}
