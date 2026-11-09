// AI-assisted adaptation of CC0 Frontline source/research, checked against GR8E69.
// Provenance, target changes and validation: docs/research/collision-batch.md.

#include "CDBGeom.h"
#include "CLine3.h"
#include "CollisionMath.h"

#ifdef DECOMP_COMPARE
EClsnState CCDBTriangle::TestCollision(const CLine3 &line, CCollision &collision, const CDBTri *tri,
                                       bool flag) const {
    const float parallelTolerance = 1e-5f;
    if (!collision.lineTest && CSurfaceType::IsSeeThru(tri->surface, false))
        return (EClsnState)1;
    CVector3 dir;
    CVector3 point;
    CVector3 normal;
    CVector3 e1;
    CVector3 e2;
    CollisionSub(e1, b, a);
    CollisionSub(e2, c, a);
    dir = line.direction;
    CVector3 p;
    CollisionCross(p, dir, e2);
    float det = CollisionDot(e1, p);
    if (!((float)fabs(det) >= parallelTolerance)) {
        if (flag && collision.result < 1)
            collision.result = 1;
        return (EClsnState)1;
    }
    float inv = 1.0f / det;
    CVector3 s;
    CollisionSub(s, line.GetStart(), a);
    float u = inv * CollisionDot(s, p);
    if (u < 0.0f || u > 1.0f) {
        if (flag && collision.result < 1)
            collision.result = 1;
        return (EClsnState)1;
    }
    CVector3 q;
    CollisionCross(q, s, e1);
    float v = inv * CollisionDot(dir, q);
    if (v < 0.0f || u + v > 1.0f) {
        if (flag && collision.result < 1)
            collision.result = 1;
        return (EClsnState)1;
    }
    float t = inv * CollisionDot(e2, q);
    if (t < 0.0f || t > 1.0f) {
        if (flag && collision.result < 1)
            collision.result = 1;
        return (EClsnState)1;
    }
    CollisionScale(point, dir, t);
    point += line.GetStart();
    collision.depth = t;
    CVector3 rest;
    CollisionSub(rest, point, line.GetEnd());
    dir *= 1.0f / line.CachedLength();
    float separation = CollisionDot(dir, rest);
    int result = CollisionClassify(separation);
    if (flag) {
        if (collision.lineTest) {
            result = 2;
            separation = 0.0f;
        }
        if (result > collision.result)
            collision.result = result;
        if (result == 2) {
            SClsnContact contact;
            CollisionCross(normal, e1, e2);
            CollisionNormalize(normal);
            contact.position = point;
            contact.normal = normal;
            contact.separation = separation;
            contact.feature = (int)tri;
            collision.AddContact(contact);
        }
    }
    return (EClsnState)result;
}

#endif
