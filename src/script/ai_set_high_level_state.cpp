// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "AIBehavior.h"

void BIFunc_AISetHighLevelState(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    EAIHighLevelState arg0 = (EAIHighLevelState)args[1];
    ((ISceneNode *)object)->GetAIObject()->SetHighLevelState(arg0);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_SoldierSetScriptHighLevelState(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)*(*stack - (count - 1));
    EAIHighLevelState arg0 = (EAIHighLevelState)*(*stack - (count - 2));
    ((CScriptedBehavior *)soldier->gameObject->GetSceneNode()->GetAIObject()->FindMatchingBehavior(BehaviorTypeScripted))->SetAllowedHighLevelState(arg0);
}

void BIFunc_AIChooseBehavior(int **stack, void *object) {
    int result = ((ISceneNode *)object)->GetAIObject()->ChooseBehavior();
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}

void BIFunc_AIChooseNewBehavior(int **stack, void *object) {
    int result = ((ISceneNode *)object)->GetAIObject()->ChooseNewBehavior();
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}

void BIFunc_AIStartBehavior(int **stack, void *object) {
    int result = ((ISceneNode *)object)->GetAIObject()->StartBehavior();
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}

void BIFunc_AIStopBehavior(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    bool immediate = reinterpret_cast<bool *>(args)[1];
    ((ISceneNode *)object)->GetAIObject()->StopBehavior(immediate);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_AIGetBehaviorCapability(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    ECapability arg0 = (ECapability)args[1];
    int result = ((ISceneNode *)object)->GetAIObject()->GetBehaviorCapability(arg0);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}

void BIFunc_AIClearBehavior(int **stack, void *object) {
    ((ISceneNode *)object)->GetAIObject()->ClearBehavior();
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_AIEnableBehavior(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    EBehaviorType arg0 = (EBehaviorType)args[1];
    ((ISceneNode *)object)->GetAIObject()->EnableBehavior(arg0);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_AIDisableBehavior(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    EBehaviorType arg0 = (EBehaviorType)args[1];
    ((ISceneNode *)object)->GetAIObject()->DisableBehavior(arg0);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_Soldier_AIDisableBehavior(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)*(*stack - (count - 1));
    EBehaviorType arg0 = (EBehaviorType)*(*stack - (count - 2));
    soldier->gameObject->GetSceneNode()->GetAIObject()->DisableBehavior(arg0);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_Soldier_AIEnableBehavior(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)*(*stack - (count - 1));
    EBehaviorType arg0 = (EBehaviorType)*(*stack - (count - 2));
    soldier->gameObject->GetSceneNode()->GetAIObject()->EnableBehavior(arg0);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
