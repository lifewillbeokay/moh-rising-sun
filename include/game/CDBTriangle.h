// AI-assisted scoped reconstruction from GR8E69; see docs/Collision.md. The header name
// is a project choice.
#ifndef GAME_CDBTRIANGLE_H
#define GAME_CDBTRIANGLE_H
#pragma interface
#include "CDBObject.h"

class CCDBTriangle {
  public:
    CCDBTriangle(const CDBTri &tri) {
        a.Set(tri.vertex[0]->x, tri.vertex[0]->y, tri.vertex[0]->z);
        b.Set(tri.vertex[1]->x, tri.vertex[1]->y, tri.vertex[1]->z);
        c.Set(tri.vertex[2]->x, tri.vertex[2]->y, tri.vertex[2]->z);
    }
    CVector3 GetNearestPoint(const CVector3 &, float *, float *) const;
    CVector3 a;
    CVector3 b;
    CVector3 c;
    EClsnState TestCollision(const CVolBox &, CCollision &, const CDBTri *, bool) const;
    EClsnState TestCollision(const CVolCapsule &, CCollision &, const CDBTri *, bool) const;
    EClsnState TestCollision(const CLine3 &, CCollision &, const CDBTri *, bool) const;
    EClsnState TestCollision(const CVolSphere &, CCollision &, const CDBTri *, bool) const;
    EClsnState TestCollision(const CAnimatedVolume &, CCollision &, const CDBTri *, bool) const;
};

#endif
