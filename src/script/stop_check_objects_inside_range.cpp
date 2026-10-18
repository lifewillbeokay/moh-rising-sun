// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "ScriptSchedule.h"

void BIFunc_StopCheckObjectsInsideRange(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSScheduleRegistrationRecord_struct *record = (BSScheduleRegistrationRecord_struct *)args[1];
    if (record)
        g_CheckObjectsInsideRangeSchedule.Unregister(record);
    else
        g_CheckObjectsInsideRangeSchedule.Unregister(g_pBSObject);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_StopCheckObjectsOutsideRange(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSScheduleRegistrationRecord_struct *record = (BSScheduleRegistrationRecord_struct *)args[1];
    if (record)
        g_CheckObjectsOutsideRangeSchedule.Unregister(record);
    else
        g_CheckObjectsOutsideRangeSchedule.Unregister(g_pBSObject);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_StopCheckObjectsInsideAngularRange(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSScheduleRegistrationRecord_struct *record = (BSScheduleRegistrationRecord_struct *)args[1];
    if (record)
        g_CheckObjectsInsideAngularRangeSchedule.Unregister(record);
    else
        g_CheckObjectsInsideAngularRangeSchedule.Unregister(g_pBSObject);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_StopCheckObjectsOutsideAngularRange(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSScheduleRegistrationRecord_struct *record = (BSScheduleRegistrationRecord_struct *)args[1];
    if (record)
        g_CheckObjectsOutsideAngularRangeSchedule.Unregister(record);
    else
        g_CheckObjectsOutsideAngularRangeSchedule.Unregister(g_pBSObject);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_StopPositionCheck(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSScheduleRegistrationRecord_struct *record = (BSScheduleRegistrationRecord_struct *)args[1];
    if (record)
        g_PositionSchedule.Unregister(record);
    else
        g_PositionSchedule.Unregister(g_pBSObject);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
