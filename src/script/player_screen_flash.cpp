// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneObjects.h"

void BIFunc_PlayerScreenFlash(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    unsigned char r = *(*stack - (count - 1));
    unsigned char g = *(*stack - (count - 2));
    unsigned char b = *(*stack - (count - 3));
    unsigned char a = *(*stack - (count - 4));
    float time = *(float *)(*stack - (count - 5));
    float hold = *(float *)(*stack - (count - 6));
    CFader::Flash(CColor(r, g, b, a), time, hold);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_PlayerScreenFadeIn(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    unsigned char r = *(*stack - (count - 1));
    unsigned char g = *(*stack - (count - 2));
    unsigned char b = *(*stack - (count - 3));
    unsigned char a = *(*stack - (count - 4));
    float time = *(float *)(*stack - (count - 5));
    CFader::FadeIn(CColor(r, g, b, a), time);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_PlayerScreenFadeOut(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    unsigned char r = *(*stack - (count - 1));
    unsigned char g = *(*stack - (count - 2));
    unsigned char b = *(*stack - (count - 3));
    unsigned char a = *(*stack - (count - 4));
    float time = *(float *)(*stack - (count - 5));
    CFader::FadeOut(CColor(r, g, b, a), time);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
