// AI-assisted adaptation of CC0 Frontline source/research, checked against GR8E69.
// Provenance, target changes and validation: docs/research/collision-batch.md.

#include "Box.h"
#include "Camera.h"
#include "CollisionMath.h"
#include "DrawContext.h"

#ifdef DECOMP_COMPARE
int CVolBox::TestVisibility(CDrawContext &context) const {
    context.clipFlags = 0;
    const CPlane *plane = context.sceneCamera->frustum.planes;
    for (int i = 0; i < 6; ++i, ++plane) {
        float d = CollisionDot(plane->normal, center) - plane->distance;
        d -= (float)fabs(halfSize[0] * CollisionDot(axisX, plane->normal));
        d -= (float)fabs(halfSize[1] * CollisionDot(axisY, plane->normal));
        d -= (float)fabs(halfSize[2] * CollisionDot(axisZ, plane->normal));
        if (!(d <= 0.0f))
            return 0;
    }
    return 1;
}

#endif
