// AI-assisted reconstruction of the projector's projection update; see docs/Rendering.md.
#include "Projector.h"
void CProjector::UpdateProjection() {
    float a = frustumParams[0];
    float b = frustumParams[1];
    float d = frustumParams[3];
    float c = frustumParams[2];
    projection.Perspective(a, b, c, d);
    frustum.Set(a, b, c, d);
    SetTMLocalToWorld(localToWorld);
}
