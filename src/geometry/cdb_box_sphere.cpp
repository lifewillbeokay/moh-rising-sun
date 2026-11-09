// AI-assisted adaptation of CC0 Frontline source/research, checked against GR8E69.
// Provenance, target changes and validation: docs/research/collision-batch.md.

#include "CDBGeom.h"
#include "CollisionMath.h"
#include "Sphere.h"

#ifdef DECOMP_COMPARE
EClsnState CCDBVolBox::TestCollision(const CVolSphere &sphere, CCollision &, bool) const {
    CVector3 p;
    CVector3 diff;
    CollisionSub(diff, sphere.GetCenter(), center);
    CollisionProjections(p, axisX, axisY, axisZ, diff);
    float remaining = sphere.radius * sphere.radius;
    float d = (float)fabs(p.x);
    if (!(d <= halfSize[0])) {
        d -= halfSize[0];
        remaining -= d * d;
    }
    d = (float)fabs(p.y);
    if (!(d <= halfSize[1])) {
        d -= halfSize[1];
        remaining -= d * d;
    }
    d = (float)fabs(p.z);
    if (!(d <= halfSize[2])) {
        d -= halfSize[2];
        remaining -= d * d;
    }
    int result = 2;
    if (!(remaining > 0.0f))
        result = 1;
    return (EClsnState)result;
}

#endif
