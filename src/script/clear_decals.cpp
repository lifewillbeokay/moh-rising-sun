// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneObjects.h"

void BIFunc_ClearDecals(int **stack, void *) {
    CDecalManager::ClearDecals();
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
