// AI-assisted adaptation of CC0 Frontline source/research, checked against GR8E69.
// Provenance, target changes and validation: docs/research/collision-batch.md.

#include "Box.h"
#include "CollisionMath.h"

#ifdef DECOMP_COMPARE
enum EAxis { AxisUnknown = 0 };
extern bool BoxBoxTest(float, float, EAxis, float &, float &, EAxis &);
static inline float Sign(float x) {
    union {
        float f;
        unsigned int u;
    } v;
    v.f = x;
    if (v.u & 0x7f800000)
        v.u = (v.u & 0x80000000) | 0x3f800000;
    else
        v.u = 0;
    return v.f;
}
static inline void BoxTransform(const CVolBox &box, CVector3 &out, const CVector3 &point) {
    out.x = ((point.x * box.axisX.x + point.y * box.axisY.x) + point.z * box.axisZ.x) + box.center.x;
    out.y = ((point.x * box.axisX.y + point.y * box.axisY.y) + point.z * box.axisZ.y) + box.center.y;
    out.z = ((point.x * box.axisX.z + point.y * box.axisY.z) + point.z * box.axisZ.z) + box.center.z;
}
#define FAIL()                                                                                               \
    {                                                                                                        \
        if (flag && collision.result < 1)                                                                    \
            collision.result = 1;                                                                            \
        return (EClsnState)1;                                                                                \
    }

#define TEST(a, b, p, n)                                                                                     \
    ra = a;                                                                                                  \
    rb = b;                                                                                                  \
    if (!BoxBoxTest(ra + rb, p, (EAxis)n, depth, sign, axis))                                                \
        FAIL();

#define ETEST(a, b, p, n)                                                                                    \
    ra = a;                                                                                                  \
    rb = b;                                                                                                  \
    proj = p;                                                                                                \
    if (proj != 0.0f && !BoxBoxTest(ra + rb, proj, (EAxis)n, depth, sign, axis))                             \
        FAIL();

EClsnState CVolBox::TestCollision(const CVolBox &other, CCollision &collision, bool flag) const {
    SClsnContact contact;
    CVector3 d;
    CVector3 r;
    CVector3 a0;
    CVector3 a1;
    CVector3 a2;
    CVector3 b0;
    CVector3 b1;
    CVector3 b2;
    CVector3 p;
    float sign;
    EAxis axis;
    float depth;
    float proj;
    float ra;
    float rb;
    int code;
    CollisionSub(d, CVector3(other.center), CVector3(center));
    axis = (EAxis)0;
    depth = 3.4028235e38f;
    a0 = axisX;
    a1 = axisY;
    a2 = axisZ;
    b0 = other.axisX;
    b1 = other.axisY;
    b2 = other.axisZ;
    CollisionProjections(r, a0, a1, a2, d);
    float tx = r.x;
    float ty = r.y;
    float tz = r.z;
    CollisionProjections(r, b0, b1, b2, d);
    float ux = r.x;
    float uy = r.y;
    float uz = r.z;
    CollisionProjections(r, b0, b1, b2, a0);
    float R00 = r.x;
    float R01 = r.y;
    float R02 = r.z;
    r.Set((float)fabs(r.x), (float)fabs(r.y), (float)fabs(r.z));
    float A00 = r.x;
    float A01 = r.y;
    float A02 = r.z;
    CollisionProjections(r, b0, b1, b2, a1);
    float R10 = r.x;
    float R11 = r.y;
    float R12 = r.z;
    r.Set((float)fabs(r.x), (float)fabs(r.y), (float)fabs(r.z));
    float A10 = r.x;
    float A11 = r.y;
    float A12 = r.z;
    CollisionProjections(r, b0, b1, b2, a2);
    float R20 = r.x;
    float R21 = r.y;
    float R22 = r.z;
    r.Set((float)fabs(r.x), (float)fabs(r.y), (float)fabs(r.z));
    float A20 = r.x;
    float A21 = r.y;
    float A22 = r.z;

    TEST(halfSize[0], other.halfSize[0] * A00 + other.halfSize[1] * A01 + other.halfSize[2] * A02, tx, 1);
    TEST(halfSize[1], other.halfSize[0] * A10 + other.halfSize[1] * A11 + other.halfSize[2] * A12, ty, 2);
    TEST(halfSize[2], other.halfSize[0] * A20 + other.halfSize[1] * A21 + other.halfSize[2] * A22, tz, 3);
    TEST(halfSize[0] * A00 + halfSize[1] * A10 + halfSize[2] * A20, other.halfSize[0], ux, 4);
    TEST(halfSize[0] * A01 + halfSize[1] * A11 + halfSize[2] * A21, other.halfSize[1], uy, 5);
    TEST(halfSize[0] * A02 + halfSize[1] * A12 + halfSize[2] * A22, other.halfSize[2], uz, 6);

    ETEST(halfSize[1] * A20 + halfSize[2] * A10, other.halfSize[1] * A02 + other.halfSize[2] * A01,
          R10 * tz - R20 * ty, 7);
    ETEST(halfSize[1] * A21 + halfSize[2] * A11, other.halfSize[0] * A02 + other.halfSize[2] * A00,
          R11 * tz - R21 * ty, 8);
    ETEST(halfSize[1] * A22 + halfSize[2] * A12, other.halfSize[0] * A01 + other.halfSize[1] * A00,
          R12 * tz - R22 * ty, 9);
    ETEST(halfSize[0] * A20 + halfSize[2] * A00, other.halfSize[1] * A12 + other.halfSize[2] * A11,
          R20 * tx - R00 * tz, 10);
    ETEST(halfSize[0] * A21 + halfSize[2] * A01, other.halfSize[0] * A12 + other.halfSize[2] * A10,
          R21 * tx - R01 * tz, 11);
    ETEST(halfSize[0] * A22 + halfSize[2] * A02, other.halfSize[0] * A11 + other.halfSize[1] * A10,
          R22 * tx - R02 * tz, 12);
    ETEST(halfSize[0] * A10 + halfSize[1] * A00, other.halfSize[1] * A22 + other.halfSize[2] * A21,
          R00 * ty - R10 * tx, 13);
    ETEST(halfSize[0] * A11 + halfSize[1] * A01, other.halfSize[0] * A22 + other.halfSize[2] * A20,
          R01 * ty - R11 * tx, 14);
    ETEST(halfSize[0] * A12 + halfSize[1] * A02, other.halfSize[0] * A21 + other.halfSize[1] * A20,
          R02 * ty - R12 * tx, 15);

    code = CollisionClassify(depth);
    if (flag) {
        if (code > collision.result)
            collision.result = code;
        if (code == 2) {
            contact.separation = depth;
            if (axis == 1) {
                CollisionScale(contact.normal, a0, sign);
                p.x = other.halfSize[0] * (-sign * Sign(R00));
                p.y = other.halfSize[1] * (-sign * Sign(R01));
                p.z = other.halfSize[2] * (-sign * Sign(R02));
                BoxTransform(other, contact.position, p);
            } else if (axis == 2) {
                CollisionScale(contact.normal, a1, sign);
                p.x = other.halfSize[0] * (-sign * Sign(R10));
                p.y = other.halfSize[1] * (-sign * Sign(R11));
                p.z = other.halfSize[2] * (-sign * Sign(R12));
                BoxTransform(other, contact.position, p);
            } else if (axis == 3) {
                CollisionScale(contact.normal, a2, sign);
                p.x = other.halfSize[0] * (-sign * Sign(R20));
                p.y = other.halfSize[1] * (-sign * Sign(R21));
                p.z = other.halfSize[2] * (-sign * Sign(R22));
                BoxTransform(other, contact.position, p);
            } else if (axis == 4) {
                CollisionScale(contact.normal, b0, sign);
                p.x = halfSize[0] * (sign * Sign(R00));
                p.y = halfSize[1] * (sign * Sign(R10));
                p.z = halfSize[2] * (sign * Sign(R20));
                BoxTransform(*this, contact.position, p);
            } else if (axis == 5) {
                CollisionScale(contact.normal, b1, sign);
                p.x = halfSize[0] * (sign * Sign(R01));
                p.y = halfSize[1] * (sign * Sign(R11));
                p.z = halfSize[2] * (sign * Sign(R21));
                BoxTransform(*this, contact.position, p);
            } else if (axis == 6) {
                CollisionScale(contact.normal, b2, sign);
                p.x = halfSize[0] * (sign * Sign(R02));
                p.y = halfSize[1] * (sign * Sign(R12));
                p.z = halfSize[2] * (sign * Sign(R22));
                BoxTransform(*this, contact.position, p);
            } else if (axis == 7) {
                CollisionCross(contact.normal, a0, b0);
                CollisionNormalize(contact.normal);
                contact.normal *= sign;
                p.y = halfSize[1] * (-sign * Sign(R20));
                p.z = halfSize[2] * (sign * Sign(R10));
                float qy = other.halfSize[1] * (-sign * Sign(R02));
                float qz = other.halfSize[2] * (sign * Sign(R01));
                float den = 1.0f - R00 * R00;
                if ((float)fabs(den) > 1e-5f)
                    p.x = (tx + R00 * (R10 * p.y + R20 * p.z - ux) + R01 * qy + R02 * qz) / den;
                else
                    p.x = 0.0f;
                BoxTransform(*this, contact.position, p);
            } else if (axis == 8) {
                CollisionCross(contact.normal, a0, b1);
                CollisionNormalize(contact.normal);
                contact.normal *= sign;
                p.y = halfSize[1] * (-sign * Sign(R21));
                p.z = halfSize[2] * (sign * Sign(R11));
                float qx = other.halfSize[0] * (sign * Sign(R02));
                float qz = other.halfSize[2] * (-sign * Sign(R00));
                float den = 1.0f - R01 * R01;
                if ((float)fabs(den) > 1e-5f)
                    p.x = (tx + R01 * (R11 * p.y + R21 * p.z - uy) + R00 * qx + R02 * qz) / den;
                else
                    p.x = 0.0f;
                BoxTransform(*this, contact.position, p);
            } else if (axis == 9) {
                CollisionCross(contact.normal, a0, b2);
                CollisionNormalize(contact.normal);
                contact.normal *= sign;
                p.y = halfSize[1] * (-sign * Sign(R22));
                p.z = halfSize[2] * (sign * Sign(R12));
                float qx = other.halfSize[0] * (-sign * Sign(R01));
                float qy = other.halfSize[1] * (sign * Sign(R00));
                float den = 1.0f - R02 * R02;
                if ((float)fabs(den) > 1e-5f)
                    p.x = (tx + R02 * (R12 * p.y + R22 * p.z - uz) + R00 * qx + R01 * qy) / den;
                else
                    p.x = 0.0f;
                BoxTransform(*this, contact.position, p);
            } else if (axis == 10) {
                CollisionCross(contact.normal, a1, b0);
                CollisionNormalize(contact.normal);
                contact.normal *= sign;
                p.x = halfSize[0] * (sign * Sign(R20));
                p.z = halfSize[2] * (-sign * Sign(R00));
                float qy = other.halfSize[1] * (-sign * Sign(R12));
                float qz = other.halfSize[2] * (sign * Sign(R11));
                float den = 1.0f - R10 * R10;
                if ((float)fabs(den) > 1e-5f)
                    p.y = (ty + R10 * (R00 * p.x + R20 * p.z - ux) + R11 * qy + R12 * qz) / den;
                else
                    p.y = 0.0f;
                BoxTransform(*this, contact.position, p);
            } else if (axis == 11) {
                CollisionCross(contact.normal, a1, b1);
                CollisionNormalize(contact.normal);
                contact.normal *= sign;
                p.x = halfSize[0] * (sign * Sign(R21));
                p.z = halfSize[2] * (-sign * Sign(R01));
                float qx = other.halfSize[0] * (sign * Sign(R12));
                float qz = other.halfSize[2] * (-sign * Sign(R10));
                float den = 1.0f - R11 * R11;
                if ((float)fabs(den) > 1e-5f)
                    p.y = (ty + R11 * (R01 * p.x + R21 * p.z - uy) + R10 * qx + R12 * qz) / den;
                else
                    p.y = 0.0f;
                BoxTransform(*this, contact.position, p);
            } else if (axis == 12) {
                CollisionCross(contact.normal, a1, b2);
                CollisionNormalize(contact.normal);
                contact.normal *= sign;
                p.x = halfSize[0] * (sign * Sign(R22));
                p.z = halfSize[2] * (-sign * Sign(R02));
                float qx = other.halfSize[0] * (-sign * Sign(R11));
                float qy = other.halfSize[1] * (sign * Sign(R10));
                float den = 1.0f - R12 * R12;
                if ((float)fabs(den) > 1e-5f)
                    p.y = (ty + R12 * (R02 * p.x + R22 * p.z - uz) + R10 * qx + R11 * qy) / den;
                else
                    p.y = 0.0f;
                BoxTransform(*this, contact.position, p);
            } else if (axis == 13) {
                CollisionCross(contact.normal, a2, b0);
                CollisionNormalize(contact.normal);
                contact.normal *= sign;
                p.x = halfSize[0] * (-sign * Sign(R10));
                p.y = halfSize[1] * (sign * Sign(R00));
                float qy = other.halfSize[1] * (-sign * Sign(R22));
                float qz = other.halfSize[2] * (sign * Sign(R21));
                float den = 1.0f - R10 * R10;
                if ((float)fabs(den) > 1e-5f)
                    p.z = (tz + R20 * (R00 * p.x + R10 * p.y - ux) + R21 * qy + R22 * qz) / den;
                else
                    p.z = 0.0f;
                BoxTransform(*this, contact.position, p);
            } else if (axis == 14) {
                CollisionCross(contact.normal, a2, b1);
                CollisionNormalize(contact.normal);
                contact.normal *= sign;
                p.x = halfSize[0] * (-sign * Sign(R11));
                p.y = halfSize[1] * (sign * Sign(R01));
                float qx = other.halfSize[0] * (sign * Sign(R22));
                float qz = other.halfSize[2] * (-sign * Sign(R20));
                float den = 1.0f - R21 * R21;
                if ((float)fabs(den) > 1e-5f)
                    p.z = (tz + R21 * (R01 * p.x + R11 * p.y - uy) + R20 * qx + R22 * qz) / den;
                else
                    p.z = 0.0f;
                BoxTransform(*this, contact.position, p);
            } else if (axis == 15) {
                CollisionCross(contact.normal, a2, b2);
                CollisionNormalize(contact.normal);
                contact.normal *= sign;
                p.x = halfSize[0] * (-sign * Sign(R12));
                p.y = halfSize[1] * (sign * Sign(R02));
                float qx = other.halfSize[0] * (-sign * Sign(R21));
                float qy = other.halfSize[1] * (sign * Sign(R20));
                float den = 1.0f - R22 * R22;
                if ((float)fabs(den) > 1e-5f)
                    p.z = (tz + R22 * (R02 * p.x + R12 * p.y - uz) + R20 * qx + R21 * qy) / den;
                else
                    p.z = 0.0f;
                BoxTransform(*this, contact.position, p);
            }
            contact.feature = 0;
            collision.AddContact(contact);
        }
    }
    return (EClsnState)code;
}

#endif
