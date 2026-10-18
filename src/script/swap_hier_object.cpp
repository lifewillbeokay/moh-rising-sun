// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"

void SwapHierObject(CHierObject *, int, int);

void BIFunc_SwapHierObject(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int first = *(*stack - (count - 1));
    int second = *(*stack - (count - 2));
    SwapHierObject(((ISceneNode *)object)->AsHierObject(), first, second);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
