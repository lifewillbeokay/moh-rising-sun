// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "AITargeting.h"

void BIFunc_EnterAStarVolume(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)*(*stack - (count - 1));
    int volume = *(*stack - (count - 2));
    CAIObject *ai = soldier->gameObject->GetSceneNode()->GetAIObject();
    if (ai)
        ai->areaPathFinding.EnterAStarVolume(volume);
}

void BIFunc_ExitAStarVolume(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)*(*stack - (count - 1));
    int volume = *(*stack - (count - 2));
    CAIObject *ai = soldier->gameObject->GetSceneNode()->GetAIObject();
    if (ai)
        ai->areaPathFinding.ExitAStarVolume(volume);
}
