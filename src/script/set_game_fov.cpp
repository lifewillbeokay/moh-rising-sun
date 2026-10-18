// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "CameraShake.h"

void BIFunc_SetGameFOV(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    float fov = *(float *)(*stack - (count - 1));
    int mode = *(*stack - (count - 2));
    CPlayerObject *player = g_scene.GetPlayer(0);
    player->fov = fov;
    player->fovMode = mode;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
