// AI-assisted reconstruction of the mesh particle system's shutdown; see docs/ParticleRecipes.md.
#include "ParticleRecipe.h"
void MeshParticleSystem::Reload() {}
void MeshParticleSystem::DeActivate() {
    flags.dying = 1;
    flags.eternal = 0;
    flags.active = 0;
    deactivationTicks = definition->contents->entries[6]->value.scalar + definition->contents->entries[7]->value.scalar;
}
void MeshParticleSystem::Terminate() {
    flags.running = 0;
    ReleaseParticles();
    MarkForDestruction(1);
}
