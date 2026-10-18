// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneObjects.h"

void BIFunc_SetAnimationStateOnOther(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    const char *state = (const char *)*(*stack - (count - 1));
    int flags = *(*stack - (count - 2));
    CAnimObjectInterface *other = (CAnimObjectInterface *)*(*stack - (count - 3));
    other->SetAnimationState(state, flags);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
