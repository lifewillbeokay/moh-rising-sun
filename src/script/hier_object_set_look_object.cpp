// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void BIFunc_HierObjectSetLookObject(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int look = args[1];
    ((CStaticObjectInterface *)((ISceneNode *)object)->AsHierObject())->AsHierTankObject()->lookObject = look;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
