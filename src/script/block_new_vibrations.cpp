// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "CameraShake.h"
#include "Shell.h"

extern int g_bCoopMultiplayer;
extern int g_bInMultiplayerMode;

void BIFunc_BlockNewVibrations(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int player = 0;
    bool block = reinterpret_cast<bool *>(args)[1];
    if (g_bInMultiplayerMode || g_bCoopMultiplayer)
        player = ((IMovingSceneNode *)g_pBSObject->gameObject->GetSceneNode()->AsPlayerObject())->GetPlayerIndex();
    g_inputMgr.SetBlockVibrations(block, player);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
