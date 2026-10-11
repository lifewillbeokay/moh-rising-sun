// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
// Static-member-only interface; this does not describe instance storage.
class CDrawContext {
public:
    static int b_ReverseParticleVsTranslucentSorting;
};
void BIFunc_ReverseParticleVsTranslucentSorting(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    CDrawContext::b_ReverseParticleVsTranslucentSorting = args[1];
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
