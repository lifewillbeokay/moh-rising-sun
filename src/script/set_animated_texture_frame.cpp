// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneObjects.h"

void BIFunc_SetAnimatedTextureFrame(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int crc = *(*stack - (count - 1));
    float frame = *(float *)(*stack - (count - 2));
    if (crc)
        AnimShape::SetFrameByCRC(crc, frame);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_SetAnimatedTextureFrameRate(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int crc = *(*stack - (count - 1));
    float rate = *(float *)(*stack - (count - 2));
    if (crc)
        AnimShape::SetFrameRateByCRC(crc, rate);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
