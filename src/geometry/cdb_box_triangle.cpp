// AI-assisted adaptation of CC0 Frontline source/research, checked against GR8E69.
// Provenance, target changes and validation: docs/research/collision-batch.md.

#include "CDBCollisionInternals.h"
#include "CDBGeom.h"
#include "CTriangle.h"
#include "CollisionMath.h"

#ifdef DECOMP_COMPARE
EClsnState CCDBVolBox::TestCollision(const CTriangle &tri, CCollision &, bool) const {
    float b2e1;
    float a0e2;
    float b1e0;
    float b0e0;
    float a2e0;
    float nd;
    float b2e0;
    float gx;
    float dx;
    float b1e1;
    float c21;
    float a1e1;
    float gz;
    float fy;
    float a1n;
    float c10;
    float fz;
    float a1e2;
    float a2e1;
    CVector3 diff;
    CVector3 e0;
    CVector3 e1;
    CVector3 normal;
    CVector3 c;
    float best = 3.4028235e38f;
    EBoxTriSeperatingAxis bestAxis;
    dx = tri.a.x - center.x;
    float dy = tri.a.y - center.y;
    float dz = tri.a.z - center.z;
    float fx = tri.b.x - tri.a.x;
    fy = tri.b.y - tri.a.y;
    fz = tri.b.z - tri.a.z;
    gx = tri.c.x - tri.a.x;
    float gy = tri.c.y - tri.a.y;
    gz = tri.c.z - tri.a.z;
    diff.x = dx;
    diff.y = dy;
    diff.z = dz;
    e0.x = fx;
    e0.y = fy;
    e0.z = fz;
    e1.x = gx;
    e1.y = gy;
    e1.z = gz;
    float a0d = CollisionDot(diff, axisX);
    float a0e0 = CollisionDot(e0, axisX);
    float a0e1 = CollisionDot(e1, axisX);
    if (separate(halfSize[0], a0d, a0e0, a0e1, (EBoxTriSeperatingAxis)1, best, bestAxis))
        return (EClsnState)1;
    float a1d = CollisionDot(diff, axisY);
    a1e1 = CollisionDot(e1, axisY);
    float a1e0 = CollisionDot(e0, axisY);
    if (separate(halfSize[1], a1d, a1e0, a1e1, (EBoxTriSeperatingAxis)2, best, bestAxis))
        return (EClsnState)1;
    float a2d = CollisionDot(diff, axisZ);
    a2e1 = CollisionDot(e1, axisZ);
    a2e0 = CollisionDot(e0, axisZ);
    if (separate(halfSize[2], a2d, a2e0, a2e1, (EBoxTriSeperatingAxis)3, best, bestAxis))
        return (EClsnState)1;
    CollisionCross(normal, e0, e1);
    CollisionNormalize(normal);
    nd = CollisionDot(normal, diff);
    a1n = CollisionDot(axisY, normal);
    float a2n = CollisionDot(axisZ, normal);
    float a0n = CollisionDot(axisX, normal);
    if (separate((float)fabs(a0n) * halfSize[0] + (float)fabs(a1n) * halfSize[1] +
                     (float)fabs(a2n) * halfSize[2],
                 nd, (EBoxTriSeperatingAxis)4, best, bestAxis))
        return (EClsnState)1;
    float c00;
    float c01;
    float c02;
    b1e0 = (float)fabs(a1e0);
    b2e0 = (float)fabs(a2e0);
    CollisionCross(c, axisX, e0);
    c00 = CollisionDot(c, diff);
    if (separate(halfSize[1] * b2e0 + halfSize[2] * b1e0, c00, a0n, (EBoxTriSeperatingAxis)5, best, bestAxis))
        return (EClsnState)1;
    b1e1 = (float)fabs(a1e1);
    b2e1 = (float)fabs(a2e1);
    CollisionCross(c, axisX, e1);
    c01 = CollisionDot(c, diff);
    if (separate(halfSize[1] * b2e1 + halfSize[2] * b1e1, c01, -a0n, (EBoxTriSeperatingAxis)6, best,
                 bestAxis))
        return (EClsnState)1;
    a1e2 = a1e1 - a1e0;
    float a2e2 = a2e1 - a2e0;
    float b1e2 = (float)fabs(a1e2);
    float b2e2 = (float)fabs(a2e2);
    c02 = c01 - c00;
    if (separate(halfSize[1] * b2e2 + halfSize[2] * b1e2, c02, -a0n, (EBoxTriSeperatingAxis)7, best,
                 bestAxis))
        return (EClsnState)1;
    b0e0 = (float)fabs(a0e0);
    CollisionCross(c, axisY, e0);
    c10 = CollisionDot(c, diff);
    if (separate(halfSize[0] * b2e0 + halfSize[2] * b0e0, c10, a1n, (EBoxTriSeperatingAxis)8, best, bestAxis))
        return (EClsnState)1;
    float b0e1 = (float)fabs(a0e1);
    CollisionCross(c, axisY, e1);
    float c11 = CollisionDot(c, diff);
    if (separate(halfSize[0] * b2e1 + halfSize[2] * b0e1, c11, -a1n, (EBoxTriSeperatingAxis)9, best,
                 bestAxis))
        return (EClsnState)1;
    a0e2 = a0e1 - a0e0;
    float b0e2 = (float)fabs(a0e2);
    if (separate(halfSize[0] * b2e2 + halfSize[2] * b0e2, c11 - c10, -a1n, (EBoxTriSeperatingAxis)10, best,
                 bestAxis))
        return (EClsnState)1;
    CollisionCross(c, axisZ, e0);
    float c20 = CollisionDot(c, diff);
    if (separate(halfSize[0] * b1e0 + halfSize[1] * b0e0, c20, a2n, (EBoxTriSeperatingAxis)11, best,
                 bestAxis))
        return (EClsnState)1;
    CollisionCross(c, axisZ, e1);
    c21 = CollisionDot(c, diff);
    if (separate(halfSize[0] * b1e1 + halfSize[1] * b0e1, c21, -a2n, (EBoxTriSeperatingAxis)12, best,
                 bestAxis))
        return (EClsnState)1;
    if (separate(halfSize[0] * b1e2 + halfSize[1] * b0e2, c21 - c20, -a2n, (EBoxTriSeperatingAxis)13, best,
                 bestAxis))
        return (EClsnState)1;
    return (EClsnState)CollisionClassify(best);
}

#endif
