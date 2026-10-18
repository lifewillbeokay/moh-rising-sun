// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "ScriptTimers.h"
#include "SceneNode.h"
#include "ScriptSchedule.h"
#include "AITargeting.h"

void BIFunc_RegisterDistanceToTarget(int **stack, void *object) {
    int result = 0;
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    unsigned short time = *(*stack - (count - 1));
    int nearSquared = *(*stack - (count - 2));
    int farSquared = *(*stack - (count - 3));
    int repeat = *(*stack - (count - 4));
    nearSquared *= nearSquared;
    farSquared *= farSquared;
    CAIObject *ai = ((ISceneNode *)object)->GetAIObject();
    if (ai->targeting.HasValidTarget())
        result = (int)g_DistanceToTargetSchedule.Register(g_pBSObject, time, repeat != 0, nearSquared, farSquared, 0, 0, 0);
    else
        BSObjectTriggerEvent(g_pBSObject, 67, 0, g_pBSObject, true);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}

void BIFunc_RegisterCheckObjectsInsideAngularRange(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int type = *(*stack - (count - 1));
    int id = *(*stack - (count - 2));
    unsigned short event = *(*stack - (count - 3));
    float angle = *(float *)(*stack - (count - 4));
    int range = *(*stack - (count - 5));
    int repeat = *(*stack - (count - 6));
    BSScheduleRegistrationRecord_struct *record = g_CheckObjectsInsideAngularRangeSchedule.Register(g_pBSObject, event, repeat != 0, type, id, (int)angle, range, 0);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = (int)record;
}

void BIFunc_RegisterCheckObjectsOutsideAngularRange(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int type = *(*stack - (count - 1));
    int id = *(*stack - (count - 2));
    unsigned short event = *(*stack - (count - 3));
    float angle = *(float *)(*stack - (count - 4));
    int range = *(*stack - (count - 5));
    int repeat = *(*stack - (count - 6));
    BSScheduleRegistrationRecord_struct *record = g_CheckObjectsOutsideAngularRangeSchedule.Register(g_pBSObject, event, repeat != 0, type, id, (int)angle, range, 0);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = (int)record;
}

void BIFunc_RegisterCheckObjectsInsideRange(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int type = *(*stack - (count - 1));
    int id = *(*stack - (count - 2));
    unsigned short event = *(*stack - (count - 3));
    int inner = *(*stack - (count - 4));
    inner *= inner;
    int outer = *(*stack - (count - 5));
    outer *= outer;
    int repeat = *(*stack - (count - 6));
    BSScheduleRegistrationRecord_struct *record = g_CheckObjectsInsideRangeSchedule.Register(g_pBSObject, event, repeat != 0, type, id, inner, outer, 0);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = (int)record;
}

void BIFunc_RegisterCheckObjectsOutsideRange(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int type = *(*stack - (count - 1));
    int id = *(*stack - (count - 2));
    unsigned short event = *(*stack - (count - 3));
    int inner = *(*stack - (count - 4));
    inner *= inner;
    int outer = *(*stack - (count - 5));
    outer *= outer;
    int repeat = *(*stack - (count - 6));
    BSScheduleRegistrationRecord_struct *record = g_CheckObjectsOutsideRangeSchedule.Register(g_pBSObject, event, repeat != 0, type, id, inner, outer, 0);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = (int)record;
}

void BIFunc_RegisterCheckObjectsInsideRange_Secondary(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int type = *(*stack - (count - 1));
    int id = *(*stack - (count - 2));
    unsigned short event = *(*stack - (count - 3));
    int inner = *(*stack - (count - 4));
    inner *= inner;
    int outer = *(*stack - (count - 5));
    outer *= outer;
    int repeat = *(*stack - (count - 6));
    BSScheduleRegistrationRecord_struct *record = g_CheckObjectsInsideRangeSchedule_Secondary.Register(g_pBSObject, event, repeat != 0, type, id, inner, outer, 0);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = (int)record;
}

void BIFunc_RegisterCheckObjectsOutsideRange_Secondary(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int type = *(*stack - (count - 1));
    int id = *(*stack - (count - 2));
    unsigned short event = *(*stack - (count - 3));
    int inner = *(*stack - (count - 4));
    inner *= inner;
    int outer = *(*stack - (count - 5));
    outer *= outer;
    int repeat = *(*stack - (count - 6));
    BSScheduleRegistrationRecord_struct *record = g_CheckObjectsOutsideRangeSchedule_Secondary.Register(g_pBSObject, event, repeat != 0, type, id, inner, outer, 0);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = (int)record;
}
