// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"

void ChangeLetterboxWidth(float);

void BIFunc_ChangeLetterboxWidth(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    ChangeLetterboxWidth(reinterpret_cast<float *>(args)[1]);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
