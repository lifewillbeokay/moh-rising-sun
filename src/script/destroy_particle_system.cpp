// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ParticleRecipe.h"

void BIFunc_DestroyParticleSystem(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    IParticleSystem *system = (IParticleSystem *)args[1];
    if (system)
        system->DeActivate();
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
