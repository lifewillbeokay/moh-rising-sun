#ifndef GAME_COLLISIONMATH_H
#define GAME_COLLISIONMATH_H
// Descriptive inline operations shared by the collision reconstructions.
// These do not claim recovered helper names. See docs/research/collision-batch.md.
#include "CVector3.h"
#include <math.h>
static inline float CollisionDot(const CVector3 &a, const CVector3 &b) {
    float v = a.x * b.x + a.y * b.y;
    return v + a.z * b.z;
}
static inline void CollisionSub(CVector3 &r, const CVector3 &a, const CVector3 &b) {
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
}
static inline void CollisionSum(CVector3 &r, const CVector3 &a, const CVector3 &b) {
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
}
static inline void CollisionScale(CVector3 &r, const CVector3 &a, float s) {
    r.x = a.x * s;
    r.y = a.y * s;
    r.z = a.z * s;
}
static inline void CollisionCross(CVector3 &r, const CVector3 &a, const CVector3 &b) {
    r.x = a.y * b.z - a.z * b.y;
    r.y = a.z * b.x - a.x * b.z;
    r.z = a.x * b.y - a.y * b.x;
}
static inline float CollisionNormalize(CVector3 &v) {
    float length = sqrtf(v.LengthSquared());
    if (length != 0.0f)
        v *= 1.0f / length;
    return length;
}
static inline void CollisionProjections(CVector3 &r, const CVector3 &a, const CVector3 &b, const CVector3 &c,
                                        const CVector3 &axis) {
    r.Set(CollisionDot(a, axis), CollisionDot(b, axis), CollisionDot(c, axis));
}
static inline int CollisionClassify(float distance) {
    int result = 2;
    if (!(distance < 0.0f))
        result = 1;
    return result;
}
#endif
