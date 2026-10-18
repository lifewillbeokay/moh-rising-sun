// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "AIBehavior.h"

void BIFunc_SwordDrawn(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)args[1];
    int result = ((CSwordBehavior *)soldier->gameObject->GetSceneNode()->GetAIObject()->FindMatchingBehavior(BehaviorTypeSword))->IsSwordDrawn();
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}
