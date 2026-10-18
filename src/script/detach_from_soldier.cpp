// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"

void DetachFromSoldier(int, bool, ISceneNode *);

void BIFunc_DetachFromSoldier(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    DetachFromSoldier(*(*stack - (count - 1)), *(bool *)(*stack - (count - 2)), (ISceneNode *)object);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
