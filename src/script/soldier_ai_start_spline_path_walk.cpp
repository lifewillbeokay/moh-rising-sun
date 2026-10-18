// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "AIFilterGlobal.h"
#include "AITargeting.h"

void BIFunc_Soldier_AIStartSplinePathWalk(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)*(*stack - (count - 1));
    unsigned int id = *(*stack - (count - 2));
    bool forward = *(bool *)(*stack - (count - 3));
    CAIFilterObject *filter = soldier->gameObject->GetSceneNode()->GetAIDoodad()->filter;
    CAISplinePath *path = g_aigAIFilterGlobalObject.GetSplinePath(id, forward);
    filter->object->splinePath.StartSplinePathWalkForward(path);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
