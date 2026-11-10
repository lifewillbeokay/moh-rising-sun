// AI-assisted reconstruction from GR8E69. Part of the original Soldier_object.cpp; there is no
// Frontline counterpart. The clients let the animation modifiers ask the soldier where to look. The
// player's clip settings at +0x1150 sit in a CCamera, whose GetPosition is in that table slot.
#include "SceneObjects.h"
#include "EAGLTransform.h"
#include "Camera.h"
#include "CameraShake.h"

void CSoldierObject::AimModifierClient::GetTMLocalToWorld(EAGL::Transform &transform) const {
    ((CAnimObjectInterface *)soldier)->GetTMLocalToWorld(transform);
}

void CSoldierObject::AimClient::GetTarget(COORD3 &out) const {
    CVector3 target;
    const CVector3 &result = target;
    ((CAnimObjectInterface *)soldier)->GetAimTarget(target);
    out.x = target.x;
    out.y = target.y;
    out.z = result.z;
}

void CSoldierObject::LookAtClient::GetTarget(COORD3 &out) const {
    CVector3 target;
    if (soldier->headTrackPlayer)
        ((CCamera *)&g_scene.GetPlayer(0)->clip)->GetPosition(target);
    else
        ((CAnimObjectInterface *)soldier)->GetAimTarget(target);
    const CVector3 &result = target;
    out.x = target.x;
    out.y = target.y;
    out.z = result.z;
}
