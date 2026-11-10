// AI-assisted reconstruction of the projector's destruction and observer callback; see docs/Rendering.md.
#include "Projector.h"
int CProjector::Destroy(CProjector *projector) {
    g_scene.Remove(*projector);
    delete projector;
    Release(projector);
    return 0;
}
void CProjector::HandleEvent(ISubject *subject, ESubjectEvent event) {
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
