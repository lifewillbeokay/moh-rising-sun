// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneObjects.h"

// Descriptive inline helper; the vector's fourth word is preserved.
static inline void ClearVector(CVector3 &v) {
    v.x = v.y = v.z = 0.0f;
}

void BIFunc_EjectHelmet(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    CVector3 velocity;
    ClearVector(velocity);
    CVector3 *impulse = &velocity;
    impulse->z = *(float *)&args[1];
    ((ISceneNode *)object)->AsSoldierObject()->EjectHelmet(velocity);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
