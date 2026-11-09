#ifndef GAME_COLLISIONPROJECTION_H
#define GAME_COLLISIONPROJECTION_H
// Descriptive inlines recovered from the Rising Sun visibility bodies.
#include "CMatrix.h"
static inline void CollisionViewPoint(CVector3 &r, const CMatrix &m,
                                      const CVector3 &p) {
  r.x = p.x * m.row[0].x + p.y * m.row[1].x + p.z * m.row[2].x + m.row[3].x;
  r.y = p.x * m.row[0].y + p.y * m.row[1].y + p.z * m.row[2].y + m.row[3].y;
  r.z = p.x * m.row[0].z + p.y * m.row[1].z + p.z * m.row[2].z + m.row[3].z;
}
static inline void CollisionClipPoint(CVector3 &r, const CMatrix &m,
                                      const CVector3 &p) {
  CollisionViewPoint(r, m, p);
  r.w = p.x * m.row[0].w + p.y * m.row[1].w + p.z * m.row[2].w + m.row[3].w;
}
#endif
