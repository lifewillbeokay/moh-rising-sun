#include "ScriptBuiltins.h"
#include "SoundAmbience.h"

// Descriptive storage annotation for the original four-byte upper-bound literal.
static const float volumeMaximum[1]
    __attribute__((section(".rodata.volume_maximum"), aligned(4))) = { 1.0f };

void BIFunc_AmbientTrack_Select(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    AmbientTrack_Select(args[1]);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_AmbientTrack_Volume(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    float volume = args[1] * 0.01f;
    if (!(volume >= 0.0f))
        volume = 0.0f;
    if (!(volume <= volumeMaximum[0]))
        volume = volumeMaximum[0];
    AmbientTrack_Volume(volume);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
