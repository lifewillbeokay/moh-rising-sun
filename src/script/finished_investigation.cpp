// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "AITargeting.h"

void BIFunc_FinishedInvestigation(int **stack, void *object) {
    ((CAIObjectInterface *)((ISceneNode *)object)->GetAIObject())->AsAISoldierObject()->threatEvaluation.FinishedInvestigation();
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
