// AI-assisted reconstruction of the mesh particle system's particle list; see docs/ParticleRecipes.md.
#include "profile.h"
#include "ParticleRecipe.h"
#include <list>
// A live particle, 112 bytes. The copy constructor establishes the layout; field names are
// placeholders. The list stores it at +16 of a 128-byte node, hence the inferred alignment.
class MeshParticle {
  public:
    CVector3 v0, v1;
    float color0[4], color1[4];
    CVector3 v2, v3;
    float unknown_60[4];
    MeshParticle() {}
    MeshParticle(const MeshParticle &);
} __attribute__((aligned(16)));
extern int g_numActiveMeshParticles;
MeshParticle *MeshParticleSystem::AllocParticle(MeshParticleList &list) {
    MeshParticle *particle = 0;
    if (g_numActiveMeshParticles < 100) {
        list.push_back(MeshParticle());
        particle = &list.back();
        g_numActiveMeshParticles++;
    }
    return particle;
}
void MeshParticleSystem::ReleaseParticle(MeshParticleList &list, MeshParticleIterator &it) {
    list.erase(it);
    g_numActiveMeshParticles--;
}
