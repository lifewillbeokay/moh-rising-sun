// AI-assisted reconstruction of the particle system's detach; see docs/ParticleRecipes.md.
#include "ParticleRecipe.h"
int CParticleSystem::Detach() {
    Assign(0);
    attachMatrix = 0;
    return 1;
}
