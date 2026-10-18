// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "ScriptSchedule.h"
#include "AITargeting.h"

void BIFunc_RegisterCorpseSearch(int **stack, void *object) {
    int zero = 0; // The original keeps the final zero argument in a saved register.
    CAIObject *ai = ((ISceneNode *)object)->GetAIObject();
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    float fov = *(float *)(*stack - (count - 1));
    float range = *(float *)(*stack - (count - 2));
    unsigned short event = *(*stack - (count - 3));
    ai->SetupFieldOfViewCorpse(fov, range);
    BSScheduleRegistrationRecord_struct *record = g_CorpseSearchSchedule.Register(g_pBSObject, event, true, 0, 0, 0, 0, zero);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = (int)record;
}
