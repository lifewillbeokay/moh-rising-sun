// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "ScriptSchedule.h"

void BIFunc_StopTargetSearch(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSScheduleRegistrationRecord_struct *record = (BSScheduleRegistrationRecord_struct *)args[1];
    if (record)
        g_TargetSearchSchedule.Unregister(record);
    else
        g_TargetSearchSchedule.Unregister((BSObject *)((ISceneNode *)object)->GetScriptObject());
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_StopTargetCheck(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSScheduleRegistrationRecord_struct *record = (BSScheduleRegistrationRecord_struct *)args[1];
    if (record)
        g_TargetCheckSchedule.Unregister(record);
    else
        g_TargetCheckSchedule.Unregister((BSObject *)((ISceneNode *)object)->GetScriptObject());
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_StopDistanceToTarget(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSScheduleRegistrationRecord_struct *record = (BSScheduleRegistrationRecord_struct *)args[1];
    if (record)
        g_DistanceToTargetSchedule.Unregister(record);
    else
        g_DistanceToTargetSchedule.Unregister((BSObject *)((ISceneNode *)object)->GetScriptObject());
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
