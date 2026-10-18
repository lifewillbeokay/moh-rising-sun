// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "CameraShake.h"

void BIFunc_PlayerMotionShake(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    float amount = *(float *)(*stack - (count - 1));
    float time = *(float *)(*stack - (count - 2));
    CPlayerObject *player = g_scene.GetPlayer(0);
    if (player)
        player->DoMotionShake(amount, time);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
