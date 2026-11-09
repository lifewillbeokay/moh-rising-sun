// AI-assisted reconstruction using CC0 Frontline references and GR8E69
// evidence. Provenance and validation:
// docs/research/collision-distance-traversal.md.

#include "Camera.h"
#include "CollisionMath.h"
#include "CollisionProjection.h"
#include "DrawContext.h"
#include "Sphere.h"

#ifdef DECOMP_COMPARE
int CVolSphere::TestVisibility(CDrawContext &context) const {
  const CCamera &camera = *context.sceneCamera;
  CVector3 view, low, high, clipLow, clipHigh;
  CollisionViewPoint(view, camera.worldToCamera, center);
  low.Set(view.x - radius, view.y - radius, view.z);
  high.Set(view.x + radius, view.y + radius, view.z);
  CollisionClipPoint(clipLow, camera.cameraToClip, low);
  float inverseLow = 1.0f / clipLow.w;
  CollisionClipPoint(clipHigh, camera.cameraToClip, high);
  float inverseHigh = 1.0f / clipHigh.w;
  context.screenLowX = clipLow.x * inverseLow;
  context.screenLowY = clipLow.y * inverseLow;
  context.screenHighX = clipHigh.x * inverseHigh;
  context.screenHighY = clipHigh.y * inverseHigh;
  context.clipFlags = 0;
  unsigned int outside = 0;
  {
    const CPlane &plane = camera.frustum.planes[0];
    float d = CollisionDot(plane.normal, center) - plane.distance;
    if (d + radius > 0.0f)
      context.clipFlags |= 1u << 26;
    if (d - radius > 0.0f)
      outside |= 1u << 26;
  }
  {
    const CPlane &plane = camera.frustum.planes[1];
    float d = CollisionDot(plane.normal, center) - plane.distance;
    if (d + radius > 0.0f)
      context.clipFlags |= 1u << 27;
    if (d - radius > 0.0f)
      outside |= 1u << 27;
  }
  {
    const CPlane &plane = camera.frustum.planes[2];
    float d = CollisionDot(plane.normal, center) - plane.distance;
    if (d + radius > 0.0f)
      context.clipFlags |= 1u << 30;
    if (d - radius > 0.0f)
      outside |= 1u << 30;
  }
  {
    const CPlane &plane = camera.frustum.planes[3];
    float d = CollisionDot(plane.normal, center) - plane.distance;
    if (d + radius > 0.0f)
      context.clipFlags |= 1u << 31;
    if (d - radius > 0.0f)
      outside |= 1u << 31;
  }
  {
    const CPlane &plane = camera.frustum.planes[4];
    float d = CollisionDot(plane.normal, center) - plane.distance;
    if (d + radius > 0.0f)
      context.clipFlags |= 1u << 29;
    if (d - radius > 0.0f)
      outside |= 1u << 29;
  }
  {
    const CPlane &plane = camera.frustum.planes[5];
    float d = CollisionDot(plane.normal, center) - plane.distance;
    if (d + radius > 0.0f)
      context.clipFlags |= 1u << 28;
    if (d - radius > 0.0f)
      outside |= 1u << 28;
  }
  return outside == 0;
}

#endif
