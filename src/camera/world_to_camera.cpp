// AI-assisted reconstruction from GR8E69; see docs/Camera.md.
#include "Camera.h"
static inline float DotComponents(const CVector3 &a, const CVector3 &b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}
void CCamera::UpdateWorldToCamera() {
    CVector3 right = localToWorld.GetRight();
    CVector3 front = localToWorld.GetFront();
    CVector3 up = localToWorld.GetUp();
    CVector3 position = localToWorld.GetPos();
    // Retain the observed component view and temporary-vector lifetimes.
    const float *rightComponents = Vector3Components(right);
    worldToCamera.SetRight(CVector3(rightComponents[0], up.x, front.x));
    worldToCamera.SetFront(CVector3(rightComponents[1], up.y, front.y));
    worldToCamera.SetUp(CVector3(rightComponents[2], up.z, front.z));
    CVector3 translation;
    translation.Set(-DotComponents(right, position), -DotComponents(up, position), -DotComponents(front, position));
    worldToCamera.SetPos(translation);
    worldToCameraValid = 1;
}
