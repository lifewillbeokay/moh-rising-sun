// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"

void SlowMOH_Start(float, float, float, float, BSObject *);

void BIFunc_StartSlowMOH(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    SlowMOH_Start(*(*stack - (count - 1)), *(*stack - (count - 2)), *(*stack - (count - 3)),
                  *(float *)(*stack - (count - 4)), g_pBSObject);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
