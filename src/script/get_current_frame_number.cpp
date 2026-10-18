// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"

extern int g_logicalFrameNum;

void BIFunc_GetCurrentFrameNumber(int **stack, void *) {
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = g_logicalFrameNum;
}
