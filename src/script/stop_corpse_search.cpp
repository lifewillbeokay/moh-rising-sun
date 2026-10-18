// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "ScriptSchedule.h"

void BIFunc_StopCorpseSearch(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSScheduleRegistrationRecord_struct *record = (BSScheduleRegistrationRecord_struct *)args[1];
    if (record)
        g_CorpseSearchSchedule.Unregister(record);
    else
        g_CorpseSearchSchedule.Unregister((BSObject *)((ISceneNode *)object)->GetScriptObject());
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
