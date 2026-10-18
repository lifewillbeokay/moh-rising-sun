// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "AITargeting.h"

void BIFunc_AISetHearingRadius(int **stack, void *object) {
    CAIObject *ai = ((ISceneNode *)object)->GetAIObject();
    ai->hearingRadius = reinterpret_cast<float *>(*stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount)[1];
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_AIGetSoldierMatchListStruct(int **stack, void *object) {
    int *matchList = &((ISceneNode *)object)->GetAIObject()->matchList;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = (int)matchList;
}
