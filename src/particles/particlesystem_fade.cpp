// AI-assisted reconstruction of the particle system's alpha fade setup; see docs/ParticleRecipes.md.
#include "ParticleRecipe.h"
void CParticleSystem::SetupAlphaFade() {
    if (UseFade()) {
        float start, end;
        GetParticleFade(start, end);
        float scale = 1.0f / (end - 1.0f);
        fadeInScale = 1.0f / start;
        fadeOutScale = scale;
        fadeOutOffset = 1.0f - end * scale;
    } else {
        fadeInScale = 1.0f;
        fadeOutOffset = 0.0f;
        fadeOutScale = 1.0f;
    }
}
