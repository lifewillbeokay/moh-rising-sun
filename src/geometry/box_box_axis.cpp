// AI-assisted adaptation of CC0 Frontline source/research, checked against GR8E69.
// Provenance, target changes and validation: docs/research/collision-batch.md.

#include "CollisionMath.h"

#ifdef DECOMP_COMPARE
enum EAxis { AxisUnknown = 0 };
static inline float Sign(float x) {
    unsigned int u = *(unsigned int *)&x;
    if (u & 0x7f800000)
        u = (u & 0x80000000) | 0x3f800000;
    else
        u = 0;
    x = *(float *)&u;
    return x;
}
static bool BoxBoxTest(float extent, float projection, EAxis axis, float &depth, float &sign,
                       EAxis &bestAxis) {
    float d = (float)fabs(projection) - extent;
    if (CollisionClassify(d) == 2) {
        if (!(d >= depth)) {
            depth = d;
            sign = Sign(projection);
            bestAxis = axis;
        }
        return true;
    }
    return false;
}

#endif
