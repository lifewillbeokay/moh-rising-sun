// AI-assisted adaptation of CC0 Frontline source/research, checked against GR8E69.
// Provenance, target changes and validation: docs/research/collision-batch.md.

#include "CDBGeom.h"
#include "Capsule.h"
#include "CollisionMath.h"

#ifdef DECOMP_COMPARE
static inline void SetStartDirection(CLine3 &line, const CVector3 &start, const CVector3 &direction) {
    line.start = start;
    line.direction = direction;
    CollisionSum(line.end, start, direction);
    line.lengthValid = line.lengthSquaredValid = 0;
}
#define EDGE0(rparam, sparam, dist)                                                                          \
    SetStartDirection(seg, this->a, e0);                                                                     \
    line.GetSquaredDistanceBetweenSegments(seg, rparam, sparam, &dist)
#define EDGE1(rparam, sparam, dist)                                                                          \
    SetStartDirection(seg, this->a, e1);                                                                     \
    line.GetSquaredDistanceBetweenSegments(seg, rparam, sparam, &dist)
#define EDGE2(rparam, sparam, dist)                                                                          \
    SetStartDirection(seg, this->b, e2);                                                                     \
    line.GetSquaredDistanceBetweenSegments(seg, rparam, sparam, &dist)
#define POINT(p)                                                                                             \
    pt = GetNearestPoint(p, &fS0, &fT0);                                                                     \
    CollisionSub(delta, p, pt);                                                                              \
    fSqrDist0 = delta.LengthSquared()
#define TAKE()                                                                                               \
    if (fSqrDist0 < fSqrDist) {                                                                              \
        fSqrDist = fSqrDist0;                                                                                \
        fR = fR0;                                                                                            \
        fS = fS0;                                                                                            \
        fT = fT0;                                                                                            \
    }

EClsnState CCDBTriangle::TestCollision(const CVolCapsule &capsule, CCollision &collision, const CDBTri *tri,
                                       bool flag) const {
    float fSqrDist;
    float fSqrDist0;
    float fR;
    float fS;
    float fT;
    float fR0;
    float fS0;
    float fT0;
    CLine3 seg;
    SClsnContact contact;
    CVector3 pt;
    CVector3 delta;
    const CLine3 &line = capsule.line;
    CVector3 start = line.GetStart();
    CVector3 end = line.GetEnd();
    CVector3 dir = line.direction;
    CVector3 diff;
    CVector3 e0;
    CVector3 e1;
    CVector3 e2;
    CVector3 normal;
    CollisionSub(diff, a, start);
    CollisionSub(e0, b, a);
    CollisionSub(e1, c, a);
    CollisionSub(e2, c, b);
    CollisionCross(normal, e0, e1);
    float n2 = normal.LengthSquared();
    // Preserve the separately rounded direction products in the distance solver.
    float directionSquares[3] = {dir.x * dir.x, dir.y * dir.y, dir.z * dir.z};
    float d2 = (directionSquares[0] + directionSquares[1]) + directionSquares[2];
    float nd = CollisionDot(normal, dir);
    if (!(nd * nd <= (n2 * d2) * 3.0461696951533668e-6f)) {
        CVector3 a;
        CollisionProjections(a, dir, e0, e1, dir);
        float fA00 = d2;
        float fA01 = -a.y;
        float fA02 = -a.z;
        float fA11 = e0.LengthSquared();
        float fA12 = CollisionDot(e0, e1);
        float fA22 = e1.LengthSquared();
        float fB0 = -CollisionDot(diff, dir);
        float fB1 = CollisionDot(diff, e0);
        float fB2 = CollisionDot(diff, e1);
        float fCof00 = fA11 * fA22 - fA12 * fA12;
        float fCof01 = fA12 * fA02 - fA01 * fA22;
        float fCof02 = fA01 * fA12 - fA11 * fA02;
        float fCof11 = fA00 * fA22 - fA02 * fA02;
        float fCof12 = fA02 * fA01 - fA00 * fA12;
        float fCof22 = fA00 * fA11 - fA01 * fA01;
        float fInvDet = 1.0f / (fA00 * fCof00 + fA01 * fCof01 + fA02 * fCof02);
        float fRhs0 = -fB0 * fInvDet;
        float fRhs1 = -fB1 * fInvDet;
        float fRhs2 = -fB2 * fInvDet;
        fR = fCof00 * fRhs0 + fCof01 * fRhs1 + fCof02 * fRhs2;
        fS = fCof01 * fRhs0 + fCof11 * fRhs1 + fCof12 * fRhs2;
        fT = fCof02 * fRhs0 + fCof12 * fRhs1 + fCof22 * fRhs2;
        if (fR < 0.0f) {
            if (fS + fT <= 1.0f) {
                if (fS < 0.0f) {
                    if (fT < 0.0f) {
                        EDGE1(fR, fT, fSqrDist);
                        fS = 0.0f;
                        EDGE0(fR0, fS0, fSqrDist0);
                        fT0 = 0.0f;
                        TAKE();
                        POINT(start);
                        fR0 = 0.0f;
                        TAKE();
                    } else {
                        EDGE1(fR, fT, fSqrDist);
                        fS = 0.0f;
                        POINT(start);
                        fR0 = 0.0f;
                        TAKE();
                    }
                } else if (fT < 0.0f) {
                    EDGE0(fR, fS, fSqrDist);
                    fT = 0.0f;
                    POINT(start);
                    fR0 = 0.0f;
                    TAKE();
                } else {
                    pt = GetNearestPoint(start, &fS, &fT);
                    CollisionSub(delta, start, pt);
                    fSqrDist = delta.LengthSquared();
                    fR = 0.0f;
                }
            } else {
                if (fS < 0.0f) {
                    EDGE1(fR, fT, fSqrDist);
                    fS = 0.0f;
                    EDGE2(fR0, fT0, fSqrDist0);
                    fS0 = 1.0f - fT0;
                    TAKE();
                    pt = GetNearestPoint(start, &fS0, &fT0);
                    CollisionSub(diff, start, pt);
                    fSqrDist0 = diff.LengthSquared();
                    fR0 = 0.0f;
                    TAKE();
                } else if (fT < 0.0f) {
                    EDGE0(fR, fS, fSqrDist);
                    fT = 0.0f;
                    EDGE2(fR0, fT0, fSqrDist0);
                    fS0 = 1.0f - fT0;
                    TAKE();
                    POINT(start);
                    fR0 = 0.0f;
                    TAKE();
                } else {
                    EDGE2(fR, fT, fSqrDist);
                    fS = 1.0f - fT;
                    POINT(start);
                    fR0 = 0.0f;
                    TAKE();
                }
            }
        } else if (fR <= 1.0f) {
            if (fS + fT <= 1.0f) {
                if (fS < 0.0f) {
                    if (fT < 0.0f) {
                        EDGE1(fR, fT, fSqrDist);
                        fS = 0.0f;
                        EDGE0(fR0, fS0, fSqrDist0);
                        fT0 = 0.0f;
                        TAKE();
                    } else {
                        EDGE1(fR, fT, fSqrDist);
                        fS = 0.0f;
                    }
                } else if (fT < 0.0f) {
                    EDGE0(fR, fS, fSqrDist);
                    fT = 0.0f;
                } else {
                    fSqrDist = fR * (fA00 * fR + fA01 * fS + fA02 * fT + 2.0f * fB0) +
                               fS * (fA01 * fR + fA11 * fS + fA12 * fT + 2.0f * fB1) +
                               fT * (fA02 * fR + fA12 * fS + fA22 * fT + 2.0f * fB2) + diff.LengthSquared();
                }
            } else {
                if (fS < 0.0f) {
                    EDGE1(fR, fT, fSqrDist);
                    fS = 0.0f;
                    EDGE2(fR0, fT0, fSqrDist0);
                    fS0 = 1.0f - fT0;
                    TAKE();
                } else if (fT < 0.0f) {
                    EDGE0(fR, fS, fSqrDist);
                    fT = 0.0f;
                    EDGE2(fR0, fT0, fSqrDist0);
                    fS0 = 1.0f - fT0;
                    TAKE();
                } else {
                    EDGE2(fR, fT, fSqrDist);
                    fS = 1.0f - fT;
                }
            }
        } else {
            if (fS + fT <= 1.0f) {
                if (fS < 0.0f) {
                    if (fT < 0.0f) {
                        EDGE1(fR, fT, fSqrDist);
                        fS = 0.0f;
                        EDGE0(fR0, fS0, fSqrDist0);
                        fT0 = 0.0f;
                        TAKE();
                        POINT(end);
                        fR0 = 1.0f;
                        TAKE();
                    } else {
                        EDGE1(fR, fT, fSqrDist);
                        fS = 0.0f;
                        POINT(end);
                        fR0 = 1.0f;
                        TAKE();
                    }
                } else if (fT < 0.0f) {
                    EDGE0(fR, fS, fSqrDist);
                    fT = 0.0f;
                    POINT(end);
                    fR0 = 1.0f;
                    TAKE();
                } else {
                    pt = GetNearestPoint(end, &fS, &fT);
                    CollisionSub(delta, end, pt);
                    fSqrDist = delta.LengthSquared();
                    fR = 1.0f;
                }
            } else {
                if (fS < 0.0f) {
                    EDGE1(fR, fT, fSqrDist);
                    fS = 0.0f;
                    EDGE2(fR0, fT0, fSqrDist0);
                    fS0 = 1.0f - fT0;
                    TAKE();
                    POINT(end);
                    fR0 = 1.0f;
                    TAKE();
                } else if (fT < 0.0f) {
                    EDGE0(fR, fS, fSqrDist);
                    fT = 0.0f;
                    EDGE2(fR0, fT0, fSqrDist0);
                    fS0 = 1.0f - fT0;
                    TAKE();
                    POINT(end);
                    fR0 = 1.0f;
                    TAKE();
                } else {
                    EDGE2(fR, fT, fSqrDist);
                    fS = 1.0f - fT;
                    POINT(end);
                    fR0 = 1.0f;
                    TAKE();
                }
            }
        }
    } else {
        EDGE0(fR, fS, fSqrDist);
        fT = 0.0f;
        EDGE1(fR0, fT0, fSqrDist0);
        fS0 = 0.0f;
        TAKE();
        EDGE2(fR0, fT0, fSqrDist0);
        fS0 = 1.0f - fT0;
        TAKE();
        POINT(start);
        fR0 = 0.0f;
        TAKE();
        POINT(end);
        fR0 = 1.0f;
        TAKE();
    }
    if (!(fSqrDist >= 0.0f))
        fSqrDist = 0.0f;
    float radius = capsule.GetRadius();
    int code = CollisionClassify(fSqrDist - radius * radius);
    if (flag) {
        if (code > collision.result)
            collision.result = code;
        if (code == 2) {
            CVector3 point;
            point.x = (e0.x * fS + a.x) + e1.x * fT;
            point.y = (e0.y * fS + a.y) + e1.y * fT;
            point.z = (e0.z * fS + a.z) + e1.z * fT;
            CVector3 rv;
            CollisionScale(rv, dir, fR);
            diff.x = start.x + rv.x - point.x;
            diff.y = start.y + rv.y - point.y;
            diff.z = start.z + rv.z - point.z;
            CollisionNormalize(diff);
            float radius = capsule.GetRadius();
            contact.separation = sqrtf(fSqrDist) - radius;
            contact.position = point;
            contact.normal = diff;
            contact.feature = (int)tri;
            collision.AddContact(contact);
        }
    }
    return (EClsnState)code;
}

#endif
