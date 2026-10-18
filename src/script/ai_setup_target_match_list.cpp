// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "AITargeting.h"

void BIFunc_AISetupTargetMatchList(int **stack, void *object) {
    CAIObjectInterface *ai = (CAIObjectInterface *)((ISceneNode *)object)->GetAIObject();
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    aistatus_match **list = (aistatus_match **)*(*stack - (count - 1));
    int size = *(*stack - (count - 2));
    ai->SetupTargetMatchList(*list, size);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_AIChooseTarget(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    float time = *(float *)&args[1];
    int result = ((CAIObjectInterface *)((ISceneNode *)object)->GetAIObject())->ChooseTarget(time);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}
