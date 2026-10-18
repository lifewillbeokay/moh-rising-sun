// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "AIBehavior.h"

void BIFunc_SignalBuddyLeadCheckpointReachedByPlayer(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    CBuddyLeadBehavior::HandleCheckpointReachedByPlayer(args[1]);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
