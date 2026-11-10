// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic reference.
// Part of the original hearingvolume.cpp; see docs/SceneObjects.md.
#include "HearingVolume.h"

void CHearingVolume::Init(IMovingSceneNode *node, EClsnId id, float radius, float height, BulletCallback bulletCallback) {
    transform.GetSlot(0);
    CVector3 center(0.0f, 0.0f, height);
    localVolume.center = center;
    localVolume.radius = radius;
    owner = node;
    collisionId = id;
    callback = bulletCallback;
    unknown_1c = 0;
    unknown_18 = 0;
    g_scene.Add(*this, 0);
}

void CHearingVolume::GetTMLocalToWorld(CMatrix &out) const { out = transform; }

void CHearingVolume::GetPosition(CVector3 &out) const { out = transform.GetPos(); }

void CHearingVolume::GetRightward(CVector3 &out) const { out = transform.GetRight(); }

void CHearingVolume::GetForward(CVector3 &out) const { out = transform.GetFront(); }

void CHearingVolume::GetUpward(CVector3 &out) const { out = transform.GetUp(); }

int CHearingVolume::GetLocalBoundingVolume(ISceneNode::EVolumeType) const { return (int)&localVolume; }

int CHearingVolume::GetWorldBoundingVolume(ISceneNode::EVolumeType) const { return (int)&worldVolume; }

void CHearingVolume::BeginUpdate(float) {
    if (owner) {
        owner->GetTMLocalToWorld(transform);
        previousPosition = transform.GetPos();
    }
}

void CHearingVolume::AttemptUpdate(float dt) {
    if (owner) {
        CVector3 move;
        CVector3 &scaled = move;
        CVector3 velocity = owner->GetWorldLinearVelocity();
        move.x = velocity.x * dt;
        scaled.y = velocity.y * dt;
        scaled.z = velocity.z * dt;
        transform.SetPos(previousPosition);
        transform.Translate(move);
    }
    worldVolume.TransformedCopy(localVolume, transform);
    CVector3 minimum, maximum;
    worldVolume.GetExtents(minimum, maximum);
    extentLowX->value = minimum.x;
    extentLowY->value = minimum.y;
    extentHighX->value = maximum.x;
    extentHighY->value = maximum.y;
}

void CHearingVolume::CommitUpdate() { previousPosition = transform.GetPos(); }
