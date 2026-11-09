// AI-assisted adaptation of CC0 Frontline source/research, checked against GR8E69.
// Provenance, target changes and validation: docs/research/collision-batch.md.

#include "CollisionMath.h"

#ifdef DECOMP_COMPARE
static bool boxline_testaxis(const CVector3 &axis, const CVector3 &offset, const CVector3 &dir, float half,
                             float &tNear, float &tFar) {
    float d = axis.x * dir.x + axis.y * dir.y + axis.z * dir.z;
    float e = axis.x * offset.x + axis.y * offset.y + axis.z * offset.z;
    if (!((float)fabs(d) <= 1e-05f)) {
        float inv = 1.0f / d;
        float t1 = (e + half) * inv;
        float t2 = (e - half) * inv;
        if (!(t1 <= t2)) {
            if (!(t2 <= tNear))
                tNear = t2;
            if (!(t1 >= tFar))
                tFar = t1;
        } else {
            if (!(t1 <= tNear))
                tNear = t1;
            if (!(t2 >= tFar))
                tFar = t2;
        }
        if (tNear > tFar)
            return false;
        if (!(tFar >= 0.0f))
            return false;
    } else if (e > half || e < -half) {
        return false;
    }
    return true;
}

#endif
