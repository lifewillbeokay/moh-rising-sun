#ifndef GAME_VOL_FRUSTUM_H
#define GAME_VOL_FRUSTUM_H
// AI-assisted scoped view of CVolFrustum; see docs/Rendering.md. Create allocates 304 bytes. The
// bounding sphere's center at +64 and radius at +272 come from the sphere-delegating tests.
#pragma interface
#include "Sphere.h"
class CVolFrustum : public IVolume {
  public:
    unsigned char unknown_4[60];
    CVector3 boundCenter;  // +64
    unsigned char unknown_50[0x110 - 0x50];
    float boundRadius;     // +272
    unsigned char unknown_114[304 - 276];
    virtual ~CVolFrustum();
    CVolFrustum &operator=(const CVolFrustum &);
    void Set(float, float, float, float);
    void Transform(const CMatrix &);
    virtual EClsnState TestCollision(const CAnimatedVolume &, CCollision &, bool) const;
    virtual EClsnState TestCollision(const CCSGVolume &, CCollision &, bool) const;
    virtual EClsnState TestCollision(const CWorldVolume &, CCollision &, bool) const;
    virtual EClsnState TestCollision(const CVolCapsule &, CCollision &, bool) const;
    virtual EClsnState TestCollision(const CVolFiber &, CCollision &, bool) const;
    virtual EClsnState TestCollision(const CCDBObject &, CCollision &, bool) const;
    virtual EClsnState TestCollision(const CVector3 &, CCollision &, bool) const;
    virtual EClsnState TestCollision(const CLine3 &, CCollision &, bool) const;
    virtual EClsnState TestCollision(const CPlane &, CCollision &, bool) const;
    virtual EClsnState TestCollision(const CTriangle &, CCollision &, bool) const;
    virtual int TestVisibility(CDrawContext &) const;
    virtual void GetExtents(CVector3 &, CVector3 &) const;
};
typedef char CVolFrustumSizeCheck[sizeof(CVolFrustum) == 304 ? 1 : -1];
#endif
