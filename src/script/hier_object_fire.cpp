// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void BIFunc_HierObjectFire(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int slot = args[1];
    CTankObject *tank = ((CStaticObjectInterface *)((ISceneNode *)object)->AsHierObject())->AsHierTankObject();
    if (tank)
        tank->FireSubObject(slot);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
