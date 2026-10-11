// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "Fog.h"
void BIFunc_GetFogBlue(int **stack, void *) {
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    union { float value; int bits; } result;
    result.value = ((g_fog.colour >> 8) & 255);
    **stack = result.bits;
}
