// AI-assisted reconstruction of the frustum's box and sphere tests; see docs/Rendering.md.
#include "VolFrustum.h"
#include "Box.h"
#include <math.h>
static inline float Dot(const CVector3 &a, const CVector3 &b) {
    float sum = a.x * b.x + a.y * b.y;
    sum += a.z * b.z;
    return sum;
}
// The box's projected half extent on each plane's normal against the center's distance.
EClsnState CVolFrustum::TestCollision(const CVolBox &box, CCollision &, bool) const {
    CVector3 half(box.halfSize[0], box.halfSize[1], box.halfSize[2]);
    CVector3 axisX(box.axisX), axisY(box.axisY), axisZ(box.axisZ), center(box.center);
    const CPlane *plane = planes;
    for (int i = 0; i < 6; i++, plane++) {
        CVector3 projected;
        projected.x = fabsf(Dot(axisX, plane->normal));
        projected.y = fabsf(Dot(axisY, plane->normal));
        projected.z = fabsf(Dot(axisZ, plane->normal));
        float reach = Dot(projected, half);
        float distance = Dot(plane->normal, center) - plane->distance;
        if (!(distance <= reach))
            return (EClsnState)1;
    }
    return (EClsnState)2;
}
static inline float SignedDistance(const CPlane &plane, const CVector3 &point) {
    float sum = plane.normal.x * point.x + plane.normal.y * point.y;
    sum += plane.normal.z * point.z;
    return sum - plane.distance;
}
// The bounding sphere against each plane: apart when beyond any plane, otherwise touching.
EClsnState CVolFrustum::TestCollision(const CVolSphere &sphere, CCollision &, bool) const {
    float radius = sphere.GetRadius();
    for (int i = 0; i < 6; i++) {
        const CVector3 &center = sphere.GetCenter();
        float distance = SignedDistance(planes[i], center);
        if (!(distance <= radius))
            return (EClsnState)1;
        if (!(distance >= radius) && distance > -radius)
            break;
    }
    return (EClsnState)2;
}
