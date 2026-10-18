// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneObjects.h"

void BIFunc_SetMotionBlurDuration(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    float amount = *(float *)(*stack - (count - 1));
    float fadeIn = *(float *)(*stack - (count - 2));
    float hold = *(float *)(*stack - (count - 3));
    float fadeOut = *(float *)(*stack - (count - 4));
    if (amount > 1.0f)
        amount = 1.0f;
    else if (!(amount >= 0.0f))
        amount = 0.0f;
    if (!(fadeIn >= 0.0f))
        fadeIn = 0.0f;
    if (!(hold >= 0.0f))
        hold = 0.0f;
    if (!(fadeOut >= 0.0f))
        fadeOut = 0.0f;
    MotionBlur::SetFrameBlur(amount, fadeIn, hold, fadeOut);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
