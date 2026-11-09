// AI-assisted adaptation of CC0 Frontline source/research, checked against GR8E69.
// Provenance, target changes and validation: docs/research/collision-batch.md.

#include "Box.h"
#include "CollisionMath.h"
#include "Frustum.h"

#ifdef DECOMP_COMPARE
static inline float Project(const CPlane &plane, const CVector3 &point, CVector3 &out) {
    float d = CollisionDot(plane.normal, point) - plane.distance;
    out.x = plane.normal.x * (-d) + point.x;
    out.y = plane.normal.y * (-d) + point.y;
    out.z = plane.normal.z * (-d) + point.z;
    return d;
}
EClsnState CVolBox::TestCollision(const CPlane &plane, CCollision &collision, bool flag) const {
    CVector3 a;
    CVector3 b;
    CVector3 c;
    CollisionScale(a, axisX, halfSize[0]);
    CollisionScale(b, axisY, halfSize[1]);
    CollisionScale(c, axisZ, halfSize[2]);
    CVector3 point;
    CVector3 corner = center;
    corner -= a;
    corner -= b;
    corner -= c;
    SClsnContact contact;
    CollisionScale(contact.normal, plane.normal, -1.0f);
    contact.separation = Project(plane, corner, contact.position);
#define STEP(v, op)                                                                                          \
    corner op v;                                                                                             \
    corner op v;                                                                                             \
    {                                                                                                        \
        float distance = Project(plane, corner, point);                                                      \
        if (distance < contact.separation) {                                                                 \
            contact.separation = distance;                                                                   \
            contact.position = point;                                                                        \
        }                                                                                                    \
    }
    STEP(a, +=)
    STEP(b, +=)
    STEP(a, -=)
    STEP(c, +=)
    STEP(b, -=)
    STEP(a, +=)
    STEP(b, +=)
    int result = CollisionClassify(contact.separation);
    if (flag) {
        if (result > collision.result)
            collision.result = result;
        if (result == 2) {
            contact.feature = 0;
            collision.AddContact(contact);
        }
    }
    return (EClsnState)result;
}

#endif
