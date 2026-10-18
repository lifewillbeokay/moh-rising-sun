// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "AITargeting.h"

void BIFunc_SetLocationTarget(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)*(*stack - (count - 1));
    CAITarget target = CAITargeting::GetLocationTarget(*(*stack - (count - 2)));
    soldier->gameObject->GetSceneNode()->GetAIObject()->targeting.SetNonAITarget(target.scriptObject);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
