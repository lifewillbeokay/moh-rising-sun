// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "ScriptTimers.h"
#include "SceneNode.h"
#include "ScriptSchedule.h"
#include "AITargeting.h"

void DebugMsg(const char *, ...);

void BIFunc_RegisterTargetCheck(int **stack, void *object) {
    int result = 0;
    CAIObject *ai = ((ISceneNode *)object)->GetAIObject();
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int mode = *(*stack - (count - 1));
    float a = *(float *)(*stack - (count - 2));
    float b = *(float *)(*stack - (count - 3));
    BSObject *target = (BSObject *)*(*stack - (count - 4));
    unsigned short event = *(*stack - (count - 5));
    int repeat = *(*stack - (count - 6));
    DebugMsg("Register %p\n", g_pBSObject);
    if (target && target->gameObject && target->gameObject->GetSceneNode() && target->gameObject->GetSceneNode()->GetAIObject()) {
        CAIObject *other = target->gameObject->GetSceneNode()->GetAIObject();
        switch (mode) {
        case 1:
            break;
        case 2:
            ai->SetupFieldOfViewModeObject(CAITarget(other), a, b);
            break;
        case 3:
            ai->SetupLineOfSightModeObject(CAITarget(other), a);
            break;
        case 5:
            ai->SetupLineOfSightModeObject(CAITarget(other), a);
            break;
        }
        result = (int)g_TargetCheckSchedule.Register(g_pBSObject, event, repeat != 0, mode, (int)a, (int)target, 0, 0);
    } else if (ai->targeting.IsNonAITarget())
        result = (int)g_TargetCheckSchedule.Register(g_pBSObject, event, repeat != 0, 3, (int)a, (int)target, 0, 0);
    else
        BSObjectTriggerEvent(g_pBSObject, 67, 0, g_pBSObject, true);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}
