// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "ScriptSchedule.h"

void BIFunc_StopCheckObjectsInsideRange_Secondary(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSScheduleRegistrationRecord_struct *record = (BSScheduleRegistrationRecord_struct *)args[1];
    if (record)
        g_CheckObjectsInsideRangeSchedule_Secondary.Unregister(record);
    else
        g_CheckObjectsInsideRangeSchedule_Secondary.Unregister(g_pBSObject);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_StopCheckObjectsOutsideRange_Secondary(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSScheduleRegistrationRecord_struct *record = (BSScheduleRegistrationRecord_struct *)args[1];
    if (record)
        g_CheckObjectsOutsideRangeSchedule_Secondary.Unregister(record);
    else
        g_CheckObjectsOutsideRangeSchedule_Secondary.Unregister(g_pBSObject);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
