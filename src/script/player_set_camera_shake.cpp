// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "CameraShake.h"

extern int g_bCoopMultiplayer;
extern int g_bInMultiplayerMode;
void DebugMsg(const char *, ...);

void BIFunc_PlayerSetCameraShake(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    float a = *(float *)(*stack - (count - 1));
    float b = *(float *)(*stack - (count - 2));
    float c = *(float *)(*stack - (count - 3));
    float d = *(float *)(*stack - (count - 4));
    DebugMsg("in playerSetCameraShake ");
    CPlayerObject *player = g_scene.GetPlayer(0);
    if (g_bInMultiplayerMode && g_pBSObject && g_pBSObject->gameObject && g_pBSObject->gameObject->GetSceneNode() &&
        g_pBSObject->gameObject->GetSceneNode()->AsPlayerObject())
        player = g_pBSObject->gameObject->GetSceneNode()->AsPlayerObject();
    player->SetCameraShake(a, b, c, d);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_PlayerStartCameraShake(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    float intensity = *(float *)(*stack - (count - 1));
    float time = *(float *)(*stack - (count - 2));
    CPlayerObject *player = g_scene.GetPlayer(0);
    if (g_bInMultiplayerMode && g_pBSObject && g_pBSObject->gameObject && g_pBSObject->gameObject->GetSceneNode() &&
        g_pBSObject->gameObject->GetSceneNode()->AsPlayerObject())
        player = g_pBSObject->gameObject->GetSceneNode()->AsPlayerObject();
    if (player)
        player->StartCameraShake(intensity, time);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_PlayerStopCameraShake(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    float time = *(float *)&args[1];
    CPlayerObject *player = g_scene.GetPlayer(0);
    if ((g_bInMultiplayerMode || g_bCoopMultiplayer) && g_pBSObject && g_pBSObject->gameObject && g_pBSObject->gameObject->GetSceneNode() &&
        g_pBSObject->gameObject->GetSceneNode()->AsPlayerObject())
        player = g_pBSObject->gameObject->GetSceneNode()->AsPlayerObject();
    player->StopCameraShake(time);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_PlayerStartBackgroundCameraShake(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    float intensity = *(float *)(*stack - (count - 1));
    float time = *(float *)(*stack - (count - 2));
    CPlayerObject *player = g_scene.GetPlayer(0);
    if (g_bInMultiplayerMode && g_pBSObject && g_pBSObject->gameObject && g_pBSObject->gameObject->GetSceneNode() &&
        g_pBSObject->gameObject->GetSceneNode()->AsPlayerObject())
        player = g_pBSObject->gameObject->GetSceneNode()->AsPlayerObject();
    player->StartBackgroundCameraShake(intensity, time);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_PlayerStopBackgroundCameraShake(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    float time = *(float *)&args[1];
    CPlayerObject *player = g_scene.GetPlayer(0);
    if (g_bInMultiplayerMode && g_pBSObject && g_pBSObject->gameObject && g_pBSObject->gameObject->GetSceneNode() &&
        g_pBSObject->gameObject->GetSceneNode()->AsPlayerObject())
        player = g_pBSObject->gameObject->GetSceneNode()->AsPlayerObject();
    player->StopBackgroundCameraShake(time);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
