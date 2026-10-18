// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void BIFunc_AIFireAtCurrentTarget(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    float duration = *(float *)(*stack - (count - 1));
    EFireMethod method = (EFireMethod)*(*stack - (count - 2));
    ((ISceneNode *)object)->AsSoldierObject()->FireCurrentWeapon(duration, method);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
