// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "AIBehavior.h"

void BIFunc_RegisterMountedMachineGun(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int linkCode = args[1];
    CMountedMachineGunBehavior::RegisterGun(linkCode, ((ISceneNode *)object)->AsStaticObject());
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
