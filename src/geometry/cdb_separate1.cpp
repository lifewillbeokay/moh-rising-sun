// AI-assisted adaptation of CC0 Frontline source/research, checked against GR8E69.
// Provenance, target changes and validation: docs/research/collision-batch.md.

#include "CDBCollisionAxis.h"
#include "CollisionMath.h"

static bool separate(float radius, float p, EBoxTriSeperatingAxis axis, float &best,
                     EBoxTriSeperatingAxis &bestAxis) {
    float d = (float)fabs(p) - radius;
    if (!(d < 0.0f))
        return true;
    if (!((float)fabs(d) >= (float)fabs(best))) {
        best = d;
        bestAxis = axis;
    }
    return false;
}
