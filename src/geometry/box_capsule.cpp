// AI-assisted reconstruction using CC0 Frontline references and GR8E69
// evidence. Provenance and validation:
// docs/research/collision-distance-traversal.md.

#include "Box.h"
#include "Capsule.h"
#include "CollisionMath.h"

#ifdef DECOMP_COMPARE
// Local helper's name/signature are preserved in box.cpp's symbols.
extern bool boxcapsule_testaxis(float, float, float, float);
extern float BoxSegmentSqrDistance(const CLine3 &, const CVolBox &, float *,
                                   float *, float *, float *);
EClsnState CVolBox::TestCollision(const CVolCapsule &capsule,
                                  CCollision &collision, bool flag) const {
  float radius = capsule.GetRadius();
  CVector3 start(capsule.line.GetStart()), direction(capsule.line.direction),
      offset, projected, delta;
  CollisionSub(offset, start, center);
  bool apart =
      boxcapsule_testaxis(CollisionDot(offset, axisX),
                          CollisionDot(direction, axisX), halfSize[0],
                          radius) ||
      boxcapsule_testaxis(CollisionDot(offset, axisY),
                          CollisionDot(direction, axisY), halfSize[1],
                          radius) ||
      boxcapsule_testaxis(CollisionDot(offset, axisZ),
                          CollisionDot(direction, axisZ), halfSize[2], radius);
  float t, x, y, z;
  if (!apart) {
    capsule.line.GetProjection(center, projected, t);
    CollisionSub(delta, projected, center);
    float squared = delta.LengthSquared();
    if (!(squared <= radius * radius)) {
      float distance = sqrtf(squared);
      delta *= 1.0f / distance;
      CVector3 a, b, c;
      CollisionScale(a, axisX, halfSize[0]);
      CollisionScale(b, axisY, halfSize[1]);
      CollisionScale(c, axisZ, halfSize[2]);
      float extent = (float)fabs(CollisionDot(a, delta)) +
                     (float)fabs(CollisionDot(b, delta)) +
                     (float)fabs(CollisionDot(c, delta));
      apart = !(distance - extent <= radius);
    }
  }
  if (apart) {
    if (flag)
      collision.RaiseResult(1);
    return (EClsnState)1;
  }
  float squared = BoxSegmentSqrDistance(capsule.line, *this, &t, &x, &y, &z);
  float radius2 = capsule.GetRadius();
  float separation = sqrtf(squared) - radius2;
  int result = CollisionClassify(separation);
  if (flag) {
    collision.RaiseResult(result);
    if (result == 2) {
      CVector3 onLine, onBox, scaled;
      CVector3 directionCopy(capsule.line.direction);
      CVector3 scaledDirection(directionCopy.x * t, directionCopy.y * t,
                               directionCopy.z * t);
      onLine = scaledDirection;
      onLine += CVector3(capsule.line.GetStart());
      CollisionScale(scaled, axisX, x);
      CollisionSum(onBox, center, scaled);
      onBox.Set(axisY.x * y + onBox.x, axisY.y * y + onBox.y,
                axisY.z * y + onBox.z);
      CollisionScale(scaled, axisZ, z);
      onBox += scaled;
      SClsnContact contact;
      CollisionSub(contact.normal, onLine, onBox);
      CollisionNormalize(contact.normal);
      CollisionScale(scaled, contact.normal, -capsule.GetRadius());
      CollisionSum(contact.position, onLine, scaled);
      contact.separation = separation;
      contact.feature = 0;
      collision.AddContact(contact);
    }
  }
  return (EClsnState)result;
}

#endif
