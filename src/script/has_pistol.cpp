// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "AIBehavior.h"

void BIFunc_HasPistol(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)args[1];
    CPistolBehavior *pistol = (CPistolBehavior *)soldier->gameObject->GetSceneNode()->GetAIObject()->FindMatchingBehavior(BehaviorTypePistol);
    BSObject *script = pistol->GetScriptObject();
    int result = 0;
    if (pistol->field_08 && script)
        result = 1;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}
