// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"

void BIFunc_SetHDCollision(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int highDetail = args[1];
    ISceneNode *anim = (ISceneNode *)((ISceneNode *)object)->AsAnimObject();
    if (anim->GetCollisionId() == 13 || anim->GetCollisionId() == 2) {
        if (highDetail)
            anim->SetCollisionId((EClsnId)13);
        else
            anim->SetCollisionId((EClsnId)2);
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
