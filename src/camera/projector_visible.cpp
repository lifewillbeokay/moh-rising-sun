// AI-assisted reconstruction of the projector's visibility test; see docs/Rendering.md.
#include "Projector.h"
#include "DrawContext.h"
int CProjector::IsVisible(CDrawContext &context) const {
    const CVolFrustum *volume = &frustum;
    CVolSphere sphere;
    CVolSphere *bound = &sphere;
    bound->center = volume->boundCenter;
    bound->SetRadius(volume->boundRadius);
    return context.IsSphereVisible(*(const COORD3 *)&(const CVector3 &)bound->GetCenter(), bound->radius);
}
