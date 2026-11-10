// AI-assisted reconstruction of the frustum tests that delegate to its bounding sphere; see docs/Rendering.md.
#include "DrawContext.h"
#include "VolFrustum.h"
EClsnState CVolFrustum::TestCollision(const CAnimatedVolume &other, CCollision &collision, bool flag) const {
    CVolSphere sphere;
    CVolSphere *bound = &sphere;
    bound->center = boundCenter;
    bound->SetRadius(boundRadius);
    return bound->CVolSphere::TestCollision(other, collision, flag);
}
EClsnState CVolFrustum::TestCollision(const CCSGVolume &other, CCollision &collision, bool flag) const {
    CVolSphere sphere;
    CVolSphere *bound = &sphere;
    bound->center = boundCenter;
    bound->SetRadius(boundRadius);
    return bound->CVolSphere::TestCollision(other, collision, flag);
}
EClsnState CVolFrustum::TestCollision(const CWorldVolume &other, CCollision &collision, bool flag) const {
    CVolSphere sphere;
    CVolSphere *bound = &sphere;
    bound->center = boundCenter;
    bound->SetRadius(boundRadius);
    return bound->CVolSphere::TestCollision(other, collision, flag);
}
EClsnState CVolFrustum::TestCollision(const CVolCapsule &other, CCollision &collision, bool flag) const {
    CVolSphere sphere;
    CVolSphere *bound = &sphere;
    bound->center = boundCenter;
    bound->SetRadius(boundRadius);
    return bound->CVolSphere::TestCollision(other, collision, flag);
}
EClsnState CVolFrustum::TestCollision(const CVolFiber &other, CCollision &collision, bool flag) const {
    CVolSphere sphere;
    CVolSphere *bound = &sphere;
    bound->center = boundCenter;
    bound->SetRadius(boundRadius);
    return bound->CVolSphere::TestCollision(other, collision, flag);
}
EClsnState CVolFrustum::TestCollision(const CCDBObject &other, CCollision &collision, bool flag) const {
    CVolSphere sphere;
    CVolSphere *bound = &sphere;
    bound->center = boundCenter;
    bound->SetRadius(boundRadius);
    return bound->CVolSphere::TestCollision(other, collision, flag);
}
EClsnState CVolFrustum::TestCollision(const CVector3 &other, CCollision &collision, bool flag) const {
    CVolSphere sphere;
    CVolSphere *bound = &sphere;
    bound->center = boundCenter;
    bound->SetRadius(boundRadius);
    return bound->CVolSphere::TestCollision(other, collision, flag);
}
EClsnState CVolFrustum::TestCollision(const CLine3 &other, CCollision &collision, bool flag) const {
    CVolSphere sphere;
    CVolSphere *bound = &sphere;
    bound->center = boundCenter;
    bound->SetRadius(boundRadius);
    return bound->CVolSphere::TestCollision(other, collision, flag);
}
EClsnState CVolFrustum::TestCollision(const CPlane &other, CCollision &collision, bool flag) const {
    CVolSphere sphere;
    CVolSphere *bound = &sphere;
    bound->center = boundCenter;
    bound->SetRadius(boundRadius);
    return bound->CVolSphere::TestCollision(other, collision, flag);
}
EClsnState CVolFrustum::TestCollision(const CTriangle &other, CCollision &collision, bool flag) const {
    CVolSphere sphere;
    CVolSphere *bound = &sphere;
    bound->center = boundCenter;
    bound->SetRadius(boundRadius);
    return bound->CVolSphere::TestCollision(other, collision, flag);
}
int CVolFrustum::TestVisibility(CDrawContext &context) const {
    CVolSphere sphere;
    CVolSphere *bound = &sphere;
    bound->center = boundCenter;
    bound->SetRadius(boundRadius);
    return context.IsSphereVisible(*(const COORD3 *)&(const CVector3 &)bound->GetCenter(), bound->radius);
}
void CVolFrustum::GetExtents(CVector3 &low, CVector3 &high) const {
    CVolSphere sphere;
    CVolSphere *bound = &sphere;
    bound->center = boundCenter;
    bound->SetRadius(boundRadius);
    bound->CVolSphere::GetExtents(low, high);
}
