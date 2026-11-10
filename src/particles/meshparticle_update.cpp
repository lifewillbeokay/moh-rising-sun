// AI-assisted reconstruction of the mesh particle system's per-frame update; see docs/ParticleRecipes.md.
#include "ParticleRecipe.h"
static inline void PostMultiply(CMatrix &m, const CMatrix &o) {
    CMatrix::s_TempMat = m;
    m.Multiply(o, CMatrix::s_TempMat);
}
// Follow the attached node, advance the particles, then run down the system's lifetime.
void MeshParticleSystem::BeginUpdate(float dt) {
    float elapsed = dt * 0.016666668f;
    if (flags.running) {
        if (subject) {
            CMatrix world;
            CMatrix &m = world;
            if (!CMatrix::s_ClassInit)
                CMatrix::InitClass();
            ((ISceneNode *)subject)->GetTMLocalToWorld(m);
            if (attachMatrix)
                PostMultiply(m, *attachMatrix);
            PostMultiply(m, attachOffset);
            SetTMLocalToWorld(m);
        }
        UpdateParticles(elapsed);
        if (flags.dying) {
            deactivationTicks -= elapsed;
            if (!(deactivationTicks > 0.0f))
                Terminate();
        } else {
            if (flags.active)
                AddParticles(elapsed);
            if (!flags.eternal) {
                deactivationTicks -= elapsed;
                if (!(deactivationTicks > 0.0f))
                    DeActivate();
            }
        }
    }
}
