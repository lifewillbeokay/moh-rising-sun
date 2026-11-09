// AI-assisted adaptation of CC0 Frontline source/research, checked against GR8E69.
// Provenance, target changes and validation: docs/research/collision-batch.md.

#include "CDBGeom.h"
#include "CollisionMath.h"
#include "Sphere.h"

#ifdef DECOMP_COMPARE
EClsnState CCDBTriangle::TestCollision(const CVolSphere &sphere, CCollision &collision, const CDBTri *tri,
                                       bool flag) const {
    CVector3 nearest = GetNearestPoint(sphere.center, 0, 0);
    const CVector3 &center = sphere.center;
    CVector3 diff;
    diff.x = center.x - nearest.x;
    diff.y = center.y - nearest.y;
    diff.z = center.z - nearest.z;
    float distance = sqrtf(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
    float separation = distance - sphere.radius;
    int result = CollisionClassify(separation);
    if (flag) {
        if (result > collision.result)
            collision.result = result;
        if (result == 2) {
            float scale = 1.0f / distance;
            SClsnContact contact;
            contact.normal.x = scale * diff.x;
            contact.normal.y = scale * diff.y;
            contact.normal.z = scale * diff.z;
            contact.position = nearest;
            contact.separation = separation;
            contact.feature = (int)tri;
            collision.AddContact(contact);
        }
    }
    return (EClsnState)result;
}

#endif
