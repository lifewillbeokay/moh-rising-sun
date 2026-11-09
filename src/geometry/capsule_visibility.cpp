// AI-assisted reconstruction using CC0 Frontline references and GR8E69
// evidence. Provenance and validation:
// docs/research/collision-distance-traversal.md.

#include "Camera.h"
#include "Capsule.h"
#include "CollisionMath.h"
#include "CollisionProjection.h"
#include "DrawContext.h"

#ifdef DECOMP_COMPARE
static inline unsigned int ClipCode(const CVector3 &p) {
  float limit = (float)fabs(p.w);
  unsigned int code = p.x > limit;
  if (!(p.x >= -limit))
    code |= 2;
  if (!(p.y <= limit))
    code |= 4;
  if (!(p.y >= -limit))
    code |= 8;
  if (!(p.z <= limit))
    code |= 16;
  if (!(p.z >= -limit))
    code |= 32;
  return code;
}
int CVolCapsule::TestVisibility(CDrawContext &context) const {
  CVector3 extent(radius, radius, 0.0f), start, end, lowStart, highStart,
      lowEnd, highEnd;
  const CCamera &camera = *context.sceneCamera;
  CollisionViewPoint(start, camera.worldToCamera, line.GetStart());
  CollisionViewPoint(end, camera.worldToCamera, line.GetEnd());
  CollisionSub(lowStart, start, extent);
  CollisionSum(highStart, start, extent);
  CollisionSub(lowEnd, end, extent);
  CollisionSum(highEnd, end, extent);
  CVector3 a, b, c, d;
  CollisionClipPoint(a, camera.cameraToClip, lowStart);
  CollisionClipPoint(b, camera.cameraToClip, highStart);
  CollisionClipPoint(c, camera.cameraToClip, lowEnd);
  CollisionClipPoint(d, camera.cameraToClip, highEnd);
  unsigned int ca = ClipCode(a), cb = ClipCode(b), cc = ClipCode(c),
               cd = ClipCode(d);
  unsigned int combined = ca | cb | cc | cd;
  int visible = !(combined && (ca & cb & cc & cd) == combined);
  a *= 1.0f / a.w;
  b *= 1.0f / b.w;
  c *= 1.0f / c.w;
  d *= 1.0f / d.w;
  context.screenLowX = c.x <= a.x ? c.x : a.x;
  context.screenLowY = c.y <= a.y ? c.y : a.y;
  context.screenHighX = d.x >= b.x ? d.x : b.x;
  context.screenHighY = d.y >= b.y ? d.y : b.y;
  return visible;
}

#endif
