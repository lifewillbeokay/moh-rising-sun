// AI-assisted reconstruction of the projector's transform and frustum setters; see docs/Rendering.md.
#include "Projector.h"
void CProjector::SetTMLocalToWorld(const CMatrix &m) {
    localToWorld = m;
    frustum.Set(frustumParams[0], frustumParams[1], frustumParams[2], frustumParams[3]);
    frustum.Transform(localToWorld);
}
