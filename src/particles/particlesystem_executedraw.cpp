// AI-assisted reconstruction of the particle system's deferred draw entry; see docs/ParticleRecipes.md.
#include "ParticleRecipe.h"
void CParticleSystem::ExecuteDraw(CDrawContext &context, void **parameters) {
    ((CParticleSystem *)parameters[0])->Render(context);
}
