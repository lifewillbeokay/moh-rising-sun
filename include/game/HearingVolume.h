#ifndef GAME_HEARINGVOLUME_H
#define GAME_HEARINGVOLUME_H
// AI-assisted reconstruction from GR8E69; see docs/SceneObjects.md. The header name is a project
// choice. The class, its methods and bases come from the symbols and the original table; field names
// are descriptive.
#include "SceneNode.h"
#include "CMatrix.h"
#include "CVector3.h"
#include "Sphere.h"
#include "CollisionVolume.h"
class CBullet;
// The default CMatrix constructor checks that the class is initialized; the shared header writes that
// out at each local, so a member needs this form to run it in member order.
struct CCheckedMatrix : public CMatrix {
    CCheckedMatrix() {
        if (!CMatrix::s_ClassInit)
            CMatrix::InitClass();
    }
};
class CHearingVolume : public IMovingSceneNode {
  public:
    typedef void (*BulletCallback)(IMovingSceneNode *, CBullet *);
    CHearingVolume();
    virtual ~CHearingVolume();
    void Init(IMovingSceneNode *, EClsnId, float, float, BulletCallback);
    virtual void BeginUpdate(float);
    virtual void AttemptUpdate(float);
    virtual void CommitUpdate();
    virtual int GetLocalBoundingVolume(ISceneNode::EVolumeType) const;
    virtual int GetWorldBoundingVolume(ISceneNode::EVolumeType) const;
    virtual void GetTMLocalToWorld(CMatrix &) const;
    virtual void GetPosition(CVector3 &) const;
    virtual void GetRightward(CVector3 &) const;
    virtual void GetForward(CVector3 &) const;
    virtual void GetUpward(CVector3 &) const;
    virtual EClsnId GetCollisionId() const;
    virtual void SetCollisionId(EClsnId);
    virtual void HandleBulletCollision(CBullet *, const CCollision &);

    int unknown_18;           // +0x18; Init clears it and +0x1c.
    int unknown_1c;
    ExtentEntry *extentLowX;  // +0x20
    ExtentEntry *extentHighX; // +0x24
    ExtentEntry *extentLowY;  // +0x28
    ExtentEntry *extentHighY; // +0x2c
    int unknown_30;
    int unknown_34;
    IMovingSceneNode *owner;  // +0x38; followed each update.
    int unknown_3c;
    CCheckedMatrix transform; // +0x40
    CVector3 previousPosition; // +0x80
    CVolSphere localVolume;   // +0x90
    CVolSphere worldVolume;   // +0xb0
    EClsnId collisionId;      // +0xd0
    BulletCallback callback;  // +0xd4; takes the owner and the bullet.
};
#endif
