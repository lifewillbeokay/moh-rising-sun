// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void BIFunc_HierObjectMapWeapon(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int weapon = *(*stack - (count - 1));
    int slot = *(*stack - (count - 2));
    ((CStaticObjectInterface *)((ISceneNode *)object)->AsHierObject())->AsHierTankObject()->MapWeaponSlot(weapon, slot - 1);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
