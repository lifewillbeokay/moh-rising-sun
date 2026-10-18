// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "AIBehavior.h"

void BIFunc_GetMMGFeederByLinkCode(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int feeder = CMountedMachineGunBehavior::GetFeederByLinkCode(args[1]);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = feeder;
}

void BIFunc_RegisterBuddyFollowObject(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    CBuddyFollowBehavior::RegisterBuddyFollowObject(
        (BSObject *)*(*stack - (count - 1)), *(float *)(*stack - (count - 2)), *(float *)(*stack - (count - 3)),
        *(float *)(*stack - (count - 4)), *(float *)(*stack - (count - 5)));
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
