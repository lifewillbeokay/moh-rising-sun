// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void BIFunc_PlayTurnAnimation(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    const char *state = (const char *)*(*stack - (count - 1));
    int flags = *(*stack - (count - 2));
    float angle = *(float *)(*stack - (count - 3));
    CSoldierObject *soldier = ((ISceneNode *)((ISceneNode *)object)->AsAnimObject())->AsSoldierObject();
    soldier->SpecifyTurnAngle(angle);
    ((CAnimObjectInterface *)soldier)->SetAnimationState(state, flags);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
