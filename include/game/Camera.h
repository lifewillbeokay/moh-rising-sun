#ifndef GAME_CAMERA_H
#define GAME_CAMERA_H
#pragma interface
// AI-assisted scoped reconstruction from GR8E69; see docs/Camera.md.
// Field names are descriptive. This view ends at +0x164, not the full allocation.
// Unknown storage may belong to this class or its bases; never instantiate it.
#include "CMatrix.h"
#include "SceneNode.h"
class CCamera : public IMovingSceneNode {
public:
    // The observed IObserver prefix occupies the first 0x18 bytes.
    unsigned char unknown_18[0x38 - 0x18];
    float nearClip, farClip;
    float projectionX, projectionY;
    unsigned char unknown_48[8];
    CMatrix localToWorld;
    CMatrix worldToCamera;
    CMatrix cameraToClip;
    CMatrix worldToClip;
    int perspective;
    int worldToCameraValid;
    int cameraToClipValid;
    int worldToClipValid;
    int field_160; // Both projection setters write 1; broader meaning unproven.
    void GetTMLocalToWorld(CMatrix &) const;
    void GetPosition(CVector3 &) const;
    void GetRightward(CVector3 &) const;
    void GetForward(CVector3 &) const;
    void GetUpward(CVector3 &) const;
    void PreTransform(const CMatrix &);
    void Reset();
    void Transform(const CMatrix &);
    void SetTMLocalToWorld(const CMatrix &);
    void SetPosition(const CVector3 &);
    void SetBasis(const CVector3 &, const CVector3 &, const CVector3 &);
    void Move(const CVector3 &);
    void Rotate(const CVector3 &, float);
    void Orthonormalize();
    void SetPerspective(float, float);
    void SetOrthographic(float, float);
    float GetHFOV() const;
    float GetVFOV() const;
    void UpdateCameraToClip();
};
#endif
