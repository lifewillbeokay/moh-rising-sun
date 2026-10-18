// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "SoundManager.h"

void BIFunc_RemoveVocal(int **stack, void *object) {
    ISceneNode *node = (ISceneNode *)object;
    if (node && node->AsSoldierObject())
        g_npcVocalMan.RemoveVocal(node->AsSoldierObject(), 1);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_Soldier_RemoveVocal(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *target = (BSObject *)args[1];
    if (target) {
        ISceneNode *node = target->gameObject->GetSceneNode();
        if (node && node->AsSoldierObject())
            g_npcVocalMan.RemoveVocal(node->AsSoldierObject(), 1);
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
