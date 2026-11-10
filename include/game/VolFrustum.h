#ifndef GAME_VOL_FRUSTUM_H
#define GAME_VOL_FRUSTUM_H
// AI-assisted scoped view of CVolFrustum; see docs/Rendering.md. Create allocates 304 bytes. The
// bounding sphere's center at +64 and radius at +272 come from the sphere-delegating tests.
#pragma interface
#include "Sphere.h"
#include "Frustum.h"
class CVolFrustum : public IVolume {
  public:
    unsigned char unknown_4[12];
    CVector3 point16, point32, point48; // +16; default constructed, not read by the code here. Names are placeholders.
    CVector3 boundCenter;  // +64
    CPlane planes[6];      // +80
    float boundRadius;     // +272
    unsigned char unknown_114[304 - 276];
    // The first virtual named here decides where the table is emitted; none of the fragments defines it yet.
    virtual EClsnState TestCollision(const CVolBox &, CCollision &, bool) const;
    virtual ~CVolFrustum();
    virtual void TransformedCopy(const IVolume &, const CMatrix &);
    virtual EClsnState TestCollision(const IVolume &, CCollision &, bool) const;
    virtual EClsnState TestCollision(const CVolSphere &, CCollision &, bool) const;
    CVolFrustum() {}
    CVolFrustum(const CVolFrustum &);
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
