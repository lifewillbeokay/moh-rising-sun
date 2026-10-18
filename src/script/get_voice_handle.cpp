// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void BIFunc_GetVoiceHandle(int **stack, void *object) {
    int handle = -1;
    CSoldierObject *soldier = ((ISceneNode *)object)->AsSoldierObject();
    if (soldier)
        handle = soldier->voiceHandle;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = handle;
}
