// AI-assisted reconstruction from GR8E69; see docs/Camera.md.
#include "Camera.h"
void CCamera::GetPosition(CVector3 &v) const {
    v = localToWorld.GetPos();
}
void CCamera::GetRightward(CVector3 &v) const {
    v = localToWorld.GetRight();
}
void CCamera::GetForward(CVector3 &v) const {
    v = localToWorld.GetFront();
}
void CCamera::GetUpward(CVector3 &v) const {
    v = localToWorld.GetUp();
}
