// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"

int BSMessageAreInSameGroup(BSObject *, BSObject *);

void BIFunc_MatchGroup(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int same = BSMessageAreInSameGroup(g_pBSObject, (BSObject *)args[1]);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = same;
}
