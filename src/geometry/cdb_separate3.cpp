// AI-assisted adaptation of CC0 Frontline source/research, checked against GR8E69.
// Provenance, target changes and validation: docs/research/collision-batch.md.

#include "CDBCollisionAxis.h"
#include "CollisionMath.h"

static bool separate(float radius, float p0, float p1, float p2, EBoxTriSeperatingAxis axis, float &best,
                     EBoxTriSeperatingAxis &bestAxis) {
    float d;
    if (!(p0 < 0.0f)) {
        if (!(p1 <= p2))
            p1 = p2;
        float m = p1;
        d = p0 - radius;
        if (!(m >= 0.0f))
            d += m;
    } else {
        if (!(p1 >= p2))
            p1 = p2;
        float m = p1;
        d = -p0 - radius;
        if (!(m <= 0.0f))
            d -= m;
    }
    if (!(d < 0.0f))
        return true;
    if (!((float)fabs(d) >= (float)fabs(best))) {
        best = d;
        bestAxis = axis;
    }
    return false;
}
