// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "CameraShake.h"

extern int g_bInMultiplayerMode;

void BIFunc_IsPlayerCrouching(int **stack, void *) {
    CPlayerObject *player;
    if (g_bInMultiplayerMode)
        player = g_pBSObject->gameObject->GetSceneNode()->AsPlayerObject();
    else
        player = g_scene.GetPlayer(0);
    int result = player->IsPlayerCrouching();
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}

void BIFunc_SetPlayerFallingDamage(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    bool enable = reinterpret_cast<bool *>(args)[1];
    g_scene.GetPlayer(0)->SetFallingDamageState(enable);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
