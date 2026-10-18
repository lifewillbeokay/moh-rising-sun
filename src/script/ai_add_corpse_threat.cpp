// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "AITargeting.h"

void BIFunc_AIAddCorpseThreat(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *corpse = (BSObject *)args[1];
    ((CAIObjectInterface *)((ISceneNode *)object)->GetAIObject())->AsAISoldierObject()->threatEvaluation.AddCorpseThreat(corpse);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
