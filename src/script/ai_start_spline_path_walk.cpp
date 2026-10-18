// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "AIFilterGlobal.h"
#include "AITargeting.h"

void BIFunc_AIStartSplinePathWalk(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    unsigned int id = *(*stack - (count - 1));
    bool forward = *(bool *)(*stack - (count - 2));
    CAIFilterObject *filter = ((ISceneNode *)object)->GetAIDoodad()->filter;
    CAISplinePath *path = g_aigAIFilterGlobalObject.GetSplinePath(id, forward);
    filter->object->splinePath.StartSplinePathWalkForward(path);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
