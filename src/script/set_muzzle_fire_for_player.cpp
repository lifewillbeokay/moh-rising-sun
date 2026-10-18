// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "CameraShake.h"

void SetMuzzleStatusForPlayer(bool, int);

void BIFunc_SetMuzzleFireForPlayer(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int enable = args[1];
    CPlayerObject *player = ((ISceneNode *)object)->AsPlayerObject();
    SetMuzzleStatusForPlayer(enable != 0, ((IMovingSceneNode *)player)->GetPlayerIndex());
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
