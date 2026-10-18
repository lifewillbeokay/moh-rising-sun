// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "AIBehavior.h"

void BIFunc_AIIsBehaviorEnabled(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    EBehaviorType arg0 = (EBehaviorType)args[1];
    int result = ((ISceneNode *)object)->GetAIObject()->IsBehaviorEnabled(arg0);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}
