// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ParticleRecipe.h"

void BIFunc_SetParticleSystemVelocity(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    IParticleSystem *system = (IParticleSystem *)*(*stack - (count - 1));
    float x = *(float *)(*stack - (count - 2));
    float y = *(float *)(*stack - (count - 3));
    float z = *(float *)(*stack - (count - 4));
    if (system) {
        CParticleSystem *procedural = system->AsProceduralParticleSystem();
        if (procedural) {
            CVector3 velocity(x, y, z);
            procedural->SetSystemInitialVelocity(velocity);
        }
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
