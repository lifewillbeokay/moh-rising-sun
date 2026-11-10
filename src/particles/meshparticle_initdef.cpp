// AI-assisted reconstruction of the mesh particle system's setup from its definition; see docs/ParticleRecipes.md.
#include "ParticleRecipe.h"
float MathFunRandomReal(float, float);
void MeshParticleSystem::Init(CParticleDef *def) {
    definition = def->AsMeshDef();
    localToWorld.GetSlot(0);
    flags.eternal = 0;
    flags.active = 1;
    flags.dying = 0;
    flags.running = 1;
    unknown_e4 = 0.0f;
    unknown_e8 = definition->contents->entries[9]->value.scalar;
    radius = 1.0f;
    flags.destroyable = 1;
    float variance = definition->contents->entries[5]->value.scalar;
    deactivationTicks = definition->contents->entries[4]->value.scalar;
    deactivationTicks += MathFunRandomReal(-variance, variance);
    if (definition->contents->entries[4]->value.scalar == 0.0f)
        flags.eternal = 1;
    else
        flags.eternal = 0;
    Reload();
}
