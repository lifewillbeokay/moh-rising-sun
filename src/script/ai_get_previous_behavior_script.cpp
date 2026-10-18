// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "AIBehavior.h"

void BIFunc_AIGetPreviousBehaviorScript(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)args[1];
    BSObject *script = 0;
    if (soldier)
        script = soldier->gameObject->GetSceneNode()->GetAIObject()->previousBehaviorScript;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = (int)script;
}

void BIFunc_AISetPreviousBehaviorScript(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)*(*stack - (count - 1));
    BSObject *script = (BSObject *)*(*stack - (count - 2));
    if (soldier)
        soldier->gameObject->GetSceneNode()->GetAIObject()->previousBehaviorScript = script;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
