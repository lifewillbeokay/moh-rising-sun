// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "SceneObjects.h"
#include "SoundManager.h"

void BIFunc_StopCurrentVoice(int **stack, void *object) {
    int result = 1;
    CSoldierObject *soldier = ((ISceneNode *)object)->AsSoldierObject();
    if (soldier && soldier->voiceHandle != -12) {
        SoundManagerView *manager = &g_SoundMan;
        manager->schedule.RemoveSound(soldier->voiceHandle);
        soldier->voiceHandle = -12;
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}
