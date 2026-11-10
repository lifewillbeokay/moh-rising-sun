// AI-assisted reconstruction of the mesh particle system's detach; see docs/ParticleRecipes.md.
#include "ParticleRecipe.h"
int MeshParticleSystem::Detach() {
    Assign(0);
    attachMatrix = 0;
    return 1;
}
