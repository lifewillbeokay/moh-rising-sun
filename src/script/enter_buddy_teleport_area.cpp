// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "AIBehavior.h"

void BIFunc_EnterBuddyTeleportArea(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)*(*stack - (count - 1));
    int area = *(*stack - (count - 2));
    ISceneNode *node = (ISceneNode *)soldier->gameObject->GetSceneNode()->AsSoldierObject();
    if (node) {
        CAIObject *ai = node->GetAIObject();
        if (ai)
            ((CBuddyFollowBehavior *)ai->FindMatchingBehavior(BehaviorTypeBuddyFollow))->EnterBuddyTeleportArea(area);
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_ExitBuddyTeleportArea(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)*(*stack - (count - 1));
    int area = *(*stack - (count - 2));
    ISceneNode *node = (ISceneNode *)soldier->gameObject->GetSceneNode()->AsSoldierObject();
    if (node) {
        CAIObject *ai = node->GetAIObject();
        if (ai)
            ((CBuddyFollowBehavior *)ai->FindMatchingBehavior(BehaviorTypeBuddyFollow))->ExitBuddyTeleportArea(area);
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_IsBuddyInArea(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)*(*stack - (count - 1));
    int arg0 = (int)*(*stack - (count - 2));
    int result = ((CBuddyFollowBehavior *)soldier->gameObject->GetSceneNode()->GetAIObject()->FindMatchingBehavior(BehaviorTypeBuddyFollow))->IsBuddyInArea(arg0);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}

void BIFunc_StartScriptedBehavior(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)args[1];
    int result = 0;
    if (soldier && soldier->gameObject && soldier->gameObject->GetSceneNode() &&
        soldier->gameObject->GetSceneNode()->GetAIObject()) {
        CScriptedBehavior *scripted = (CScriptedBehavior *)soldier->gameObject->GetSceneNode()->GetAIObject()->FindMatchingBehavior(BehaviorTypeScripted);
        if (scripted) {
            result = 1;
            scripted->StartScript(g_pBSObject);
        }
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}

void BIFunc_StopScriptedBehavior(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)args[1];
    if (soldier)
        ((CScriptedBehavior *)soldier->gameObject->GetSceneNode()->GetAIObject()->FindMatchingBehavior(BehaviorTypeScripted))->StopScript();
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_SetScriptedBehaviorCapability(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)*(*stack - (count - 1));
    ECapability capability = (ECapability)*(*stack - (count - 2));
    bool enable = *(bool *)(*stack - (count - 3));
    if (soldier)
        ((CScriptedBehavior *)soldier->gameObject->GetSceneNode()->GetAIObject()->FindMatchingBehavior(BehaviorTypeScripted))->SetCapability(capability, enable);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
