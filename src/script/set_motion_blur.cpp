// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneObjects.h"

void BIFunc_SetMotionBlur(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    float amount = *(float *)(*stack - (count - 1));
    float decay = *(float *)(*stack - (count - 2));
    if (amount > 1.0f)
        amount = 1.0f;
    else if (!(amount >= 0.0f))
        amount = 0.0f;
    if (!(decay >= 0.0f))
        decay = 0.0f;
    MotionBlur::SetFrameBlur(amount, decay, 0.0f, 0.0f);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
