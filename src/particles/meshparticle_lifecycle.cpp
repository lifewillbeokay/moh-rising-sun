// AI-assisted reconstruction of the mesh particle system's destruction and observer callback; see docs/ParticleRecipes.md.
#include "ParticleRecipe.h"
void MeshParticleSystem::MarkForDestruction(int mode) {
    if (flags.destroyable) {
        ISubject::MarkForDestruction(mode);
        g_scene.Remove(*this);
    }
}
void MeshParticleSystem::Destroy() {
    if (flags.destroyable) {
        ReleaseParticles();
        ReleaseSystem(this);
        delete this;
    }
}
void MeshParticleSystem::HandleEvent(ISubject *subject, ESubjectEvent event) {
    switch ((int)event) {
    case 1:
    case 2: {
        CMatrix m;
        CMatrix *p = &m;
        if (!CMatrix::s_ClassInit)
            CMatrix::InitClass();
        ((ISceneNode *)subject)->GetTMLocalToWorld(*p);
        SetTMLocalToWorld(*p);
        break;
    }
    case 4:
    case 8:
        Detach();
        MarkForDestruction(3);
        break;
    }
}
