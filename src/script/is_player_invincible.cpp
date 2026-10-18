// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "CameraShake.h"

void BIFunc_IsPlayerInvincible(int **stack, void *) {
    int invincible = g_scene.GetPlayer(0)->IsInvincible();
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = invincible;
}
