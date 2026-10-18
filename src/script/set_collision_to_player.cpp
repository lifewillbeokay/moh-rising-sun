// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"

void BIFunc_SetCollisionToPlayer(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int toPlayer = args[1];
    ISceneNode *anim = (ISceneNode *)((ISceneNode *)object)->AsAnimObject();
    if (toPlayer)
        anim->SetCollisionId((EClsnId)2);
    else
        anim->SetCollisionId((EClsnId)25);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
