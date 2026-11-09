// AI-assisted reconstruction using CC0 Frontline references and GR8E69
// evidence. Provenance and validation:
// docs/research/collision-distance-traversal.md.

#include "Box.h"
#include "CollisionMath.h"
#include "Sphere.h"

#ifdef DECOMP_COMPARE
static inline float CollisionSign(float x) {
  union {
    float f;
    unsigned int u;
  } value;
  value.f = x;
  value.u = (value.u & 0x7f800000) ? ((value.u & 0x80000000) | 0x3f800000) : 0;
  return value.f;
}
EClsnState CVolBox::TestCollision(const CVolSphere &sphere,
                                  CCollision &collision, bool flag) const {
  CVector3 diff, p, absolute;
  CollisionSub(diff, sphere.GetCenter(), center);
  CollisionProjections(p, axisX, axisY, axisZ, diff);
  absolute.Set((float)fabs(p.x), (float)fabs(p.y), (float)fabs(p.z));
  float insideDistance = -3.402823466e38f;
  float squareDistance = 0.0f;
  bool outside = false;
  CVector3 normal(0.0f, 0.0f, 0.0f), point(p);
  for (int i = 0; i < 3; ++i) {
    float d = (&absolute.x)[i] - halfSize[i];
    if (!(d <= 0.0f)) {
      squareDistance += d * d;
      outside = true;
      if (flag) {
        float sign = CollisionSign((&p.x)[i]);
        (&normal.x)[i] = sign * d;
        (&point.x)[i] = sign * halfSize[i];
      }
    } else if (d > insideDistance)
      insideDistance = d;
  }
  float radius = sphere.GetRadius();
  float separation =
      (outside ? sqrtf(squareDistance) : insideDistance) - radius;
  int result = CollisionClassify(separation);
  if (flag) {
    collision.RaiseResult(result);
    if (result == 2) {
      SClsnContact contact;
      contact.normal.Set(
          axisX.x * normal.x + axisY.x * normal.y + axisZ.x * normal.z,
          axisX.y * normal.x + axisY.y * normal.y + axisZ.y * normal.z,
          axisX.z * normal.x + axisY.z * normal.y + axisZ.z * normal.z);
      CollisionNormalize(contact.normal);
      contact.position.Set(
          axisX.x * point.x + axisY.x * point.y + axisZ.x * point.z + center.x,
          axisX.y * point.x + axisY.y * point.y + axisZ.y * point.z + center.y,
          axisX.z * point.x + axisY.z * point.y + axisZ.z * point.z + center.z);
      contact.separation = separation;
      contact.feature = 0;
      collision.AddContact(contact);
    }
  }
  return (EClsnState)result;
}

#endif
