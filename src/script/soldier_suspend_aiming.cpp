// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void BIFunc_Soldier_SuspendAiming(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)args[1];
    soldier->gameObject->GetSceneNode()->AsSoldierObject()->aimSuspended = 1;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_Soldier_ResumeAiming(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)args[1];
    soldier->gameObject->GetSceneNode()->AsSoldierObject()->ResumeAimModifier();
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_ActivatePropAiming(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    bool enable = reinterpret_cast<bool *>(args)[1];
    ((ISceneNode *)object)->AsSoldierObject()->EnablePropAimModifier(enable);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_SuspendPropAiming(int **stack, void *object) {
    ((ISceneNode *)object)->AsSoldierObject()->propAimSuspended = 1;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_ResumePropAiming(int **stack, void *object) {
    ((ISceneNode *)object)->AsSoldierObject()->ResumePropAimModifier();
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_Soldier_ActivatePropAiming(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)*(*stack - (count - 1));
    bool enable = *(bool *)(*stack - (count - 2));
    soldier->gameObject->GetSceneNode()->AsSoldierObject()->EnablePropAimModifier(enable);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_Soldier_SuspendPropAiming(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)args[1];
    soldier->gameObject->GetSceneNode()->AsSoldierObject()->propAimSuspended = 1;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_Soldier_ResumePropAiming(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)args[1];
    soldier->gameObject->GetSceneNode()->AsSoldierObject()->ResumePropAimModifier();
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
