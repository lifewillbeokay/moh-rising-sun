// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "AIBehavior.h"

void BIFunc_AIFindTank(int **stack, void *) {
    int result = 0;
    CAITankObject *tank = CAITankObject::FindTank();
    if (tank)
        result = (int)tank->scriptObject;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}
