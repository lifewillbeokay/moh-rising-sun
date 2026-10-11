// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "CVector3.h"
extern "C" float sqrtf(float);
float MathFunRandomReal(float, float);

static inline void Cross(CVector3 &out, const CVector3 &a, const CVector3 &b) {
    CVector3 result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    out.x = result.x;
    out.y = result.y;
    out.z = result.z;
}

static inline void Normalize(CVector3 &v) {
    float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    if (length != 0.0f) {
        float inverse = 1.0f / length;
        v.x *= inverse;
        v.y *= inverse;
        v.z *= inverse;
    }
}

static void AdjustAim(CVector3 &direction, float spread) {
    CVector3 up(0.0f, 0.0f, 1.0f);
    CVector3 right;
    Cross(right, direction, up);
    Normalize(right);
    Cross(up, right, direction);
    Normalize(up);
    float limit = spread * 0.5f;
    float x = MathFunRandomReal(-limit, limit);
    float y = MathFunRandomReal(-limit, limit);
    right *= x;
    up *= y;
    direction += right;
    direction += up;
    Normalize(direction);
}
