// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "CameraShake.h"

void BIFunc_SetWeaponEnable(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int weapon = *(*stack - (count - 1));
    bool enable = *(bool *)(*stack - (count - 2));
    int result = ((ISceneNode *)object)->AsPlayerObject()->SetWeaponEnable(weapon, enable);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}
