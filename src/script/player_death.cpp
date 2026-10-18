// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "CameraShake.h"

extern int g_bInMultiplayerMode;

void BIFunc_PlayerDeath(int **stack, void *object) {
    CPlayerObject *player = ((ISceneNode *)object)->AsPlayerObject();
    if (g_bInMultiplayerMode) {
        player->StartDeathSequence();
        player->DropMPDumpObject();
    } else {
        g_scene.GetPlayer(0)->StartDeathSequence();
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
