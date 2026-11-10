// AI-assisted reconstruction of the particle system's observer callback; see docs/ParticleRecipes.md.
#include "ParticleRecipe.h"
void CParticleSystem::HandleEvent(ISubject *subject, ESubjectEvent event) {
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
