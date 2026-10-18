// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneObjects.h"
#include "Bullet.h"

// Descriptive storage annotation; the following literal belongs to helmet ejection.
static const float depletedValue[1]
    __attribute__((section(".rodata.depleted_value"), aligned(4))) = { 0.0f };

void BIFunc_DoDamageIfLocationMatches(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    ScriptHitInfo *hit = (ScriptHitInfo *)*(*stack - (count - 1));
    int location = *(*stack - (count - 2));
    float remaining = *(float *)(*stack - (count - 3));
    if (hit->location == location) {
        CHierObject *hierarchy = ((ISceneNode *)object)->AsHierObject();
        if (hierarchy) {
            int bulletClass = GetClassFromBulletType(hit->bulletType);
            float multiplier = hierarchy->GetBulletDamageMultiplierByClass(location, bulletClass);
            hit->damage *= multiplier;
        }
        remaining -= hit->damage;
        if (!(remaining > 0.001))
            remaining = depletedValue[0];
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    volatile float result = remaining;
    **stack = *(int *)&result;
}
