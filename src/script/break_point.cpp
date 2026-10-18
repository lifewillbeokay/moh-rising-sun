// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include <stdio.h>

void BIFunc_BreakPoint(int **stack, void *) {
    printf("break point!\n");
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
