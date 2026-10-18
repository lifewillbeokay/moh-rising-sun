// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "ScriptSchedule.h"
#include "AITargeting.h"

void BIFunc_RegisterTargetSearch(int **stack, void *object) {
    CAIObject *ai = ((ISceneNode *)object)->GetAIObject();
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int mode = *(*stack - (count - 1));
    float a = *(float *)(*stack - (count - 2));
    float b = *(float *)(*stack - (count - 3));
    aistatus_match **list = (aistatus_match **)*(*stack - (count - 4));
    int size = *(*stack - (count - 5));
    unsigned short event = *(*stack - (count - 6));
    int repeat = *(*stack - (count - 7));
    switch (mode) {
    case 2:
        ai->SetupFieldOfViewModeList(*list, size, a, b);
        break;
    case 3:
        ai->SetupLineOfSightModeList(*list, size, a);
        break;
    }
    int result = (int)g_TargetSearchSchedule.Register(g_pBSObject, event, repeat != 0, mode, 0, 0, 0, 0);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}
