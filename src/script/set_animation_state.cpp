// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void BIFunc_SetAnimationState(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    const char *state = (const char *)*(*stack - (count - 1));
    int flags = *(*stack - (count - 2));
    ((CAnimObjectInterface *)((ISceneNode *)object)->AsAnimObject())->SetAnimationState(state, flags);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
