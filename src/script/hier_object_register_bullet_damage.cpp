// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void BIFunc_HierObjectRegisterBulletDamage(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int a = *(*stack - (count - 1));
    int b = *(*stack - (count - 2));
    int c = *(*stack - (count - 3));
    float scale = *(float *)(*stack - (count - 4));
    CHierObject *hier = ((ISceneNode *)object)->AsHierObject();
    if (!(scale >= 0.0f))
        scale = 1.0f / -scale;
    hier->RegisterBulletDamage(a, b, c, scale);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
