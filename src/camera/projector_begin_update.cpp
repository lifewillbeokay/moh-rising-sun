// AI-assisted reconstruction of the projector's per-frame update; see docs/Rendering.md.
#include "Projector.h"
void CProjector::BeginUpdate(float) {
    if (subject) {
        CMatrix m;
        CMatrix *world = &m;
        if (!CMatrix::s_ClassInit)
            CMatrix::InitClass();
        ((ISceneNode *)subject)->GetTMLocalToWorld(*world);
        CMatrix *attach = attachMatrix;
        if (attach) {
            CMatrix::s_TempMat = *world;
            world->Multiply(*attach, CMatrix::s_TempMat);
        }
        const CMatrix *local = &offset;
        CMatrix::s_TempMat = *world;
        world->Multiply(*local, CMatrix::s_TempMat);
        SetTMLocalToWorld(*world);
    }
}
void CProjector::Draw(CDrawContext &) {
}
