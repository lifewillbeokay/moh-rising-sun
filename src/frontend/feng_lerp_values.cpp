// AI-assisted reconstruction from GR8E69; see docs/research/feng-animation-lists.md.
#include "FEngInterpolation.h"

void FELerpInteger(long a, long b, float t, long *base, long *out) {
    *out = *base + a + (long)((b - a) * t + 0.5f);
}

void FELerpFloat(float a, float b, float t, float *base, float *out) {
    *out = *base + a + (b - a) * t;
}

void FELerpVector2(FEVector2 &a, FEVector2 &b, float t, FEVector2 *base, FEVector2 *out) {
    out->x = base->x + a.x + (b.x - a.x) * t;
    out->y = base->y + a.y + (b.y - a.y) * t;
}

void FELerpVector3(FEVector3 &a, FEVector3 &b, float t, FEVector3 *base, FEVector3 *out) {
    out->x = base->x + a.x + (b.x - a.x) * t;
    out->y = base->y + a.y + (b.y - a.y) * t;
    out->z = base->z + a.z + (b.z - a.z) * t;
}

#ifdef DECOMP_COMPARE
// AI-assisted reconstruction from GR8E69; see docs/research/feng-animation-lists.md.
#include "FEngInterpolation.h"
#include "FEngQuaternionOps.h"
#include <math.h>

void FELerpQuaternion(FEQuaternion &a, FEQuaternion &b, float t,
                      FEQuaternion *base, FEQuaternion *out) {
    FEQuaternion next = b;
    float dot = a.x * next.x + a.y * next.y + a.z * next.z + a.w * next.w;
    if (!(dot >= 0.0f)) {
        next.x = -next.x; next.y = -next.y;
        next.z = -next.z; next.w = -next.w;
        dot = -dot;
    }
    if (!(dot >= 0.999f)) {
        float angle = acosf(dot);
        float sine = sinf(angle);
        float right = sinf(angle * t);
        float left = sinf(angle * (1.0f - t));
        next = FEQuatScale(FEQuatAdd(FEQuatScale(a, left), FEQuatScale(next, right)), 1.0f / sine);
    } else {
        // The target uses the original b here, even after negating next above.
        FEQuaternion value = FEQuatAdd(a, FEQuatScale(FEQuatSubtract(b, a), t));
        float inverse = 1.0f / sqrtf(value.x * value.x + value.y * value.y +
                                    value.z * value.z + value.w * value.w);
        if (!(inverse <= 0.00001f)) {
            value.x *= inverse; value.y *= inverse;
            value.z *= inverse; value.w *= inverse;
        } else {
            value.x = value.y = value.z = 0.0f;
            value.w = 1.0f;
        }
        next = value;
    }
    *out = FEQuatMultiply(next, *base);
}

#endif
