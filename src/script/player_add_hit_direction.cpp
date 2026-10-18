// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "CameraShake.h"

void BIFunc_PlayerAddHitDirection(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int direction = args[1];
    CPlayerObject *player = ((ISceneNode *)object)->AsPlayerObject();
    if (direction == 0)
        player->hud.AddHitDirection(1);
    else if (direction == 1)
        player->hud.AddHitDirection(3);
    else if (direction == 2)
        player->hud.AddHitDirection(4);
    else if (direction == 3)
        player->hud.AddHitDirection(2);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
