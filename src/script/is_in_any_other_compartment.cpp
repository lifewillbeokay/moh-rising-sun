// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void BIFunc_IsInAnyOtherCompartment(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int compartment = args[1];
    int result = ((ISceneNode *)object)->AsSoldierObject()->IsInAnyOtherCompartment(compartment);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}
