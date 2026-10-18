// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"

void DestroyBSObject(BSObject *);

void BIFunc_DestroyScript(int **stack, void *) {
    DestroyBSObject(g_pBSObject);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
