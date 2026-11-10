// AI-assisted reconstruction from GR8E69; see docs/research/feng-animation-lists.md.
#ifdef DECOMP_COMPARE
#include "FEngMath.h"
#include <string.h>

void FEQuaternion::GetMatrix(FEMatrix4 *out) {
    float x2 = x + x, y2 = y + y, z2 = z + z;
    float xx = x * x2, yy = y * y2, zz = z * z2;
    float xy = x * y2, xz = x * z2, yz = y * z2;
    float wx = w * x2, wy = w * y2, wz = w * z2;
    out->m[0][0] = 1.0f - (yy + zz);
    out->m[0][1] = xy + wz;
    out->m[0][2] = xz - wy;
    out->m[0][3] = 0.0f;
    out->m[1][0] = xy - wz;
    out->m[1][1] = 1.0f - (xx + zz);
    out->m[1][2] = yz + wx;
    out->m[1][3] = 0.0f;
    out->m[2][0] = xz + wy;
    out->m[2][1] = yz - wx;
    out->m[2][2] = 1.0f - (xx + yy);
    out->m[2][3] = 0.0f;
    out->m[3][0] = out->m[3][1] = out->m[3][2] = 0.0f;
    out->m[3][3] = 1.0f;
}

void FEMatrix4::Identify() {
    m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
    m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
    m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}

void FEMultMatrix(FEMatrix4 *out, const FEMatrix4 *a, const FEMatrix4 *b) {
    FEMatrix4 value;
    value.m[0][0] = a->m[0][0] * b->m[0][0] + a->m[1][0] * b->m[0][1] +
                        a->m[2][0] * b->m[0][2] + a->m[3][0] * b->m[0][3];
    value.m[0][1] = a->m[0][1] * b->m[0][0] + a->m[1][1] * b->m[0][1] +
                        a->m[2][1] * b->m[0][2] + a->m[3][1] * b->m[0][3];
    value.m[0][2] = a->m[0][2] * b->m[0][0] + a->m[1][2] * b->m[0][1] +
                        a->m[2][2] * b->m[0][2] + a->m[3][2] * b->m[0][3];
    value.m[0][3] = a->m[0][3] * b->m[0][0] + a->m[1][3] * b->m[0][1] +
                        a->m[2][3] * b->m[0][2] + a->m[3][3] * b->m[0][3];
    value.m[1][0] = a->m[0][0] * b->m[1][0] + a->m[1][0] * b->m[1][1] +
                        a->m[2][0] * b->m[1][2] + a->m[3][0] * b->m[1][3];
    value.m[1][1] = a->m[0][1] * b->m[1][0] + a->m[1][1] * b->m[1][1] +
                        a->m[2][1] * b->m[1][2] + a->m[3][1] * b->m[1][3];
    value.m[1][2] = a->m[0][2] * b->m[1][0] + a->m[1][2] * b->m[1][1] +
                        a->m[2][2] * b->m[1][2] + a->m[3][2] * b->m[1][3];
    value.m[1][3] = a->m[0][3] * b->m[1][0] + a->m[1][3] * b->m[1][1] +
                        a->m[2][3] * b->m[1][2] + a->m[3][3] * b->m[1][3];
    value.m[2][0] = a->m[0][0] * b->m[2][0] + a->m[1][0] * b->m[2][1] +
                        a->m[2][0] * b->m[2][2] + a->m[3][0] * b->m[2][3];
    value.m[2][1] = a->m[0][1] * b->m[2][0] + a->m[1][1] * b->m[2][1] +
                        a->m[2][1] * b->m[2][2] + a->m[3][1] * b->m[2][3];
    value.m[2][2] = a->m[0][2] * b->m[2][0] + a->m[1][2] * b->m[2][1] +
                        a->m[2][2] * b->m[2][2] + a->m[3][2] * b->m[2][3];
    value.m[2][3] = a->m[0][3] * b->m[2][0] + a->m[1][3] * b->m[2][1] +
                        a->m[2][3] * b->m[2][2] + a->m[3][3] * b->m[2][3];
    value.m[3][0] = a->m[0][0] * b->m[3][0] + a->m[1][0] * b->m[3][1] +
                        a->m[2][0] * b->m[3][2] + a->m[3][0] * b->m[3][3];
    value.m[3][1] = a->m[0][1] * b->m[3][0] + a->m[1][1] * b->m[3][1] +
                        a->m[2][1] * b->m[3][2] + a->m[3][1] * b->m[3][3];
    value.m[3][2] = a->m[0][2] * b->m[3][0] + a->m[1][2] * b->m[3][1] +
                        a->m[2][2] * b->m[3][2] + a->m[3][2] * b->m[3][3];
    value.m[3][3] = a->m[0][3] * b->m[3][0] + a->m[1][3] * b->m[3][1] +
                        a->m[2][3] * b->m[3][2] + a->m[3][3] * b->m[3][3];
    memcpy(out, &value, sizeof(value));
}

void FEMultMatrix(FEVector3 *out, const FEMatrix4 *m, const FEVector3 *point) {
    FEVector3 value(m->m[0][0] * point->x + m->m[1][0] * point->y + m->m[2][0] * point->z + m->m[3][0],
                    m->m[0][1] * point->x + m->m[1][1] * point->y + m->m[2][1] * point->z + m->m[3][1],
                    m->m[0][2] * point->x + m->m[1][2] * point->y + m->m[2][2] * point->z + m->m[3][2]);
    out->z = value.z; out->x = value.x; out->y = value.y;
}

#endif
