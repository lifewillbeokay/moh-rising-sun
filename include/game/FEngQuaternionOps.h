#ifndef GAME_FENG_QUATERNION_OPS_H
#define GAME_FENG_QUATERNION_OPS_H
#include "FEngMath.h"

// Descriptive operations for the value temporaries in FEng interpolation.
// These names are project choices, not recovered method names.
static inline FEQuaternion FEQuatScale(const FEQuaternion &a, float scale) {
    FEQuaternion r;
    r.x = a.x * scale; r.y = a.y * scale;
    r.z = a.z * scale; r.w = a.w * scale;
    return r;
}
static inline FEQuaternion FEQuatAdd(const FEQuaternion &a, const FEQuaternion &b) {
    FEQuaternion r;
    r.x = a.x + b.x; r.y = a.y + b.y;
    r.z = a.z + b.z; r.w = a.w + b.w;
    return r;
}
static inline FEQuaternion FEQuatSubtract(const FEQuaternion &a, const FEQuaternion &b) {
    FEQuaternion r;
    r.x = a.x - b.x; r.y = a.y - b.y;
    r.z = a.z - b.z; r.w = a.w - b.w;
    return r;
}
static inline FEQuaternion FEQuatMultiply(const FEQuaternion &a, const FEQuaternion &b) {
    FEQuaternion r;
    r.x = (a.y * b.z - a.z * b.y) + a.x * b.w + b.x * a.w;
    r.y = (a.z * b.x - a.x * b.z) + a.y * b.w + b.y * a.w;
    r.z = (a.x * b.y - a.y * b.x) + a.z * b.w + b.z * a.w;
    r.w = a.w * b.w - (a.x * b.x + a.y * b.y + a.z * b.z);
    return r;
}
#endif
