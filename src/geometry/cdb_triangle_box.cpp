// AI-assisted adaptation of CC0 Frontline source/research, checked against GR8E69.
// Provenance, target changes and validation: docs/research/collision-batch.md.

#include "CDBCollisionInternals.h"
#include "CDBGeom.h"
#include "CollisionMath.h"

#ifdef DECOMP_COMPARE
#define FAIL()                                                                                               \
    {                                                                                                        \
        if (flag && collision.result < 1)                                                                    \
            collision.result = 1;                                                                            \
        return (EClsnState)1;                                                                                \
    }

EClsnState CCDBTriangle::TestCollision(const CVolBox &box, CCollision &collision, const CDBTri *tri,
                                       bool flag) const {
    SClsnContact contact;
    CVector3 e0;
    CVector3 e1;
    CVector3 normal;
    CVector3 diff;
    CVector3 p;
    CVector3 e2;
    float h2;
    float h0;
    float h1;
    float best = 3.4028235e38f;
    EBoxTriSeperatingAxis bestAxis = (EBoxTriSeperatingAxis)0;
    CollisionSub(diff, a, box.center);
    CollisionSub(e0, b, a);
    CollisionSub(e1, c, a);
    CollisionSub(e2, c, b);
    h0 = box.halfSize[0];
    CVector3 a0 = box.axisX;
    CollisionProjections(p, diff, e0, e1, a0);
    if (separate(h0, p.x, p.y, p.z, (EBoxTriSeperatingAxis)1, best, bestAxis))
        FAIL();
    h1 = box.halfSize[1];
    CVector3 a1 = box.axisY;
    CollisionProjections(p, diff, e0, e1, a1);
    if (separate(h1, p.x, p.y, p.z, (EBoxTriSeperatingAxis)2, best, bestAxis))
        FAIL();
    h2 = box.halfSize[2];
    CVector3 a2 = box.axisZ;
    CollisionProjections(p, diff, e0, e1, a2);
    if (separate(h2, p.x, p.y, p.z, (EBoxTriSeperatingAxis)3, best, bestAxis))
        FAIL();
    CollisionCross(normal, e0, e1);
    CollisionNormalize(normal);
    CollisionProjections(p, a0, a1, a2, normal);
    p.x = fabs(p.x);
    p.y = fabs(p.y);
    p.z = fabs(p.z);
    if (separate(p.x * h0 + p.y * h1 + p.z * h2, CollisionDot(normal, diff), (EBoxTriSeperatingAxis)4, best,
                 bestAxis))
        FAIL();
    CVector3 c00;
    CollisionCross(c00, a0, e0);
    if (sqrtf(c00.LengthSquared()) > 0.001f) {
        CollisionNormalize(c00);
        float q0 = CollisionDot(c00, diff);
        float q1 = CollisionDot(c00, e1);
        if (separate(h1 * (float)fabs(CollisionDot(a1, c00)) + h2 * (float)fabs(CollisionDot(a2, c00)), q0,
                     q1, (EBoxTriSeperatingAxis)5, best, bestAxis))
            FAIL();
    }
    CVector3 c01;
    CollisionCross(c01, a0, e1);
    if (CollisionNormalize(c01) > 0.001f) {
        float q0 = CollisionDot(c01, diff);
        float q1 = CollisionDot(c01, e0);
        if (separate(h1 * (float)fabs(CollisionDot(a1, c01)) + h2 * (float)fabs(CollisionDot(a2, c01)), q0,
                     q1, (EBoxTriSeperatingAxis)6, best, bestAxis))
            FAIL();
    }
    CVector3 c02;
    CollisionCross(c02, a0, e2);
    if (CollisionNormalize(c02) > 0.001f) {
        CollisionProjections(p, diff, e0, e1, c02);
        float q0 = p.x;
        float q1 = p.y;
        float q2 = p.z;
        if (separate(h1 * (float)fabs(CollisionDot(a1, c02)) + h2 * (float)fabs(CollisionDot(a2, c02)), q0,
                     q1, q2, (EBoxTriSeperatingAxis)7, best, bestAxis))
            FAIL();
    }
    CVector3 c10;
    CollisionCross(c10, a1, e0);
    if (CollisionNormalize(c10) > 0.001f) {
        float q0 = CollisionDot(c10, diff);
        float q1 = CollisionDot(c10, e1);
        if (separate(h0 * (float)fabs(CollisionDot(a0, c10)) + h2 * (float)fabs(CollisionDot(a2, c10)), q0,
                     q1, (EBoxTriSeperatingAxis)8, best, bestAxis))
            FAIL();
    }
    CVector3 c11;
    CollisionCross(c11, a1, e1);
    if (CollisionNormalize(c11) > 0.001f) {
        float q0 = CollisionDot(c11, diff);
        float q1 = CollisionDot(c11, e0);
        if (separate(h0 * (float)fabs(CollisionDot(a0, c11)) + h2 * (float)fabs(CollisionDot(a2, c11)), q0,
                     q1, (EBoxTriSeperatingAxis)9, best, bestAxis))
            FAIL();
    }
    CVector3 c12;
    CollisionCross(c12, a1, e2);
    if (CollisionNormalize(c12) > 0.001f) {
        CollisionProjections(p, diff, e0, e1, c12);
        float q0 = p.x;
        float q1 = p.y;
        float q2 = p.z;
        if (separate(h0 * (float)fabs(CollisionDot(a0, c12)) + h2 * (float)fabs(CollisionDot(a2, c12)), q0,
                     q1, q2, (EBoxTriSeperatingAxis)10, best, bestAxis))
            FAIL();
    }
    CVector3 c20;
    CollisionCross(c20, a2, e0);
    if (CollisionNormalize(c20) > 0.001f) {
        float q0 = CollisionDot(c20, diff);
        float q1 = CollisionDot(c20, e1);
        if (separate(h0 * (float)fabs(CollisionDot(a0, c20)) + h1 * (float)fabs(CollisionDot(a1, c20)), q0,
                     q1, (EBoxTriSeperatingAxis)11, best, bestAxis))
            FAIL();
    }
    CVector3 c21;
    CollisionCross(c21, a2, e1);
    if (CollisionNormalize(c21) > 0.001f) {
        float q0 = CollisionDot(c21, diff);
        float q1 = CollisionDot(c21, e0);
        if (separate(h0 * (float)fabs(CollisionDot(a0, c21)) + h1 * (float)fabs(CollisionDot(a1, c21)), q0,
                     q1, (EBoxTriSeperatingAxis)12, best, bestAxis))
            FAIL();
    }
    CVector3 c22;
    CollisionCross(c22, a2, e2);
    if (CollisionNormalize(c22) > 0.001f) {
        CollisionProjections(p, diff, e0, e1, c22);
        float q0 = p.x;
        float q1 = p.y;
        float q2 = p.z;
        if (separate(h0 * (float)fabs(CollisionDot(a0, c22)) + h1 * (float)fabs(CollisionDot(a1, c22)), q0,
                     q1, q2, (EBoxTriSeperatingAxis)13, best, bestAxis))
            FAIL();
    }
    int code = CollisionClassify(best);
    if (flag) {
        if (code > collision.result)
            collision.result = code;
        if (code == 2) {
            contact.separation = best;
            if (bestAxis == 1)
                contact.normal = a0;
            else if (bestAxis == 2)
                contact.normal = a1;
            else if (bestAxis == 3)
                contact.normal = a2;
            else if (bestAxis == 4)
                contact.normal = normal;
            else if (bestAxis == 5)
                contact.normal = c00;
            else if (bestAxis == 6)
                contact.normal = c01;
            else if (bestAxis == 7)
                contact.normal = c02;
            else if (bestAxis == 8)
                contact.normal = c10;
            else if (bestAxis == 9)
                contact.normal = c11;
            else if (bestAxis == 10)
                contact.normal = c12;
            else if (bestAxis == 11)
                contact.normal = c20;
            else if (bestAxis == 12)
                contact.normal = c21;
            else if (bestAxis == 13)
                contact.normal = c22;
            contact.feature = (int)tri;
            collision.AddContact(contact);
        }
    }
    return (EClsnState)code;
}

#endif
