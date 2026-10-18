// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "SceneObjects.h"
#include "CameraShake.h"

void BIFunc_PathMechAllowCollisionDamage(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int allow = args[1];
    ((ISceneNode *)object)->AsStaticObject()->flags.allowCollisionDamage = allow;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_SetHeadAsRoot(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    ((CSoldierObject *)object)->SetHeadTrans(reinterpret_cast<bool *>(args)[1]);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_SetPlayerCollision(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    EPlayerCollisionType type = (EPlayerCollisionType)args[1];
    CPlayerObject *player = g_scene.GetPlayer(0);
    if (player)
        ((ISceneNode *)player)->SetCollisionId(GetCollisionIDFromScriptCollisionType(type));
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
