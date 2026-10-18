// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "CameraShake.h"

void BIFunc_IsPlayerAiming(int **stack, void *object) {
    CPlayerObject *player = ((ISceneNode *)object)->AsPlayerObject();
    int aiming = (player->pathFlags >> 27) & 1;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = aiming;
}
