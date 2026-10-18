// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"

void BIFunc_DisableCollision(int **stack, void *object) {
    ((ISceneNode *)((ISceneNode *)object)->AsAnimObject())->SetCollisionId((EClsnId)-1);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
