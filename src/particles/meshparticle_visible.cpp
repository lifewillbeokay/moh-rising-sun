// AI-assisted reconstruction of the mesh particle system's visibility and transform; see docs/ParticleRecipes.md.
#include "DrawContext.h"
#include "ParticleRecipe.h"
int MeshParticleSystem::IsVisible(CDrawContext &context) const {
    return context.IsSphereVisible(*(const COORD3 *)center, radius);
}
unsigned int MeshParticleSystem::IsDrawEnabled() const { return 1; }
void MeshParticleSystem::SetTMLocalToWorld(const CMatrix &m) { localToWorld = m; }
