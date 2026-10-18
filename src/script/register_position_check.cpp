// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "ScriptSchedule.h"

void BIFunc_RegisterPositionCheck(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int id = *(*stack - (count - 1));
    float range = *(float *)(*stack - (count - 2));
    unsigned short event = *(*stack - (count - 3));
    BSScheduleRegistrationRecord_struct *record =
        g_PositionSchedule.Register(g_pBSObject, event, true, id, (int)range, 0, 0, 0);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = (int)record;
}
