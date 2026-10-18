// AI-assisted scoped reconstruction; see docs/SceneNode.md.
#ifndef MOH_SCENENODE_H
#define MOH_SCENENODE_H
#pragma interface
#include "ObserverTypes.h"

class CBullet;
class CAIDoodad;
class CAIObject;
class CAnimObject;
class CAnimatedPlayer;
class CBotObject;
class CCollisionVolume;
class CHierObject;
class CPlayerObject;
class CPlayerWeaponObject;
class CProjector;
class CSoldierObject;
class CStaticObject;
class CWorldObject;
class CCollision;
class CDrawContext;
class CLight;
class CMatrix;
class CVector3;
class BPDLightVolume;
class IMovingSceneNode;
// Parameter enum; its members are unknown.
enum EClsnId { ClsnIdUnknown = 0 };

// Virtual slots 5-67, in the order of the original ISceneNode table at 0x802e1680.
// Names and parameter types come from original symbols. Return types are not
// encoded in symbols: pointer returns follow the casts' and accessors' overrides,
// and int returns are ABI placeholders; see docs/SceneNode.md. Data members are
// not declared; complete size and layout remain unknown.
class ISceneNode : public IObserver {
  public:
    enum EVolumeType { VolumeTypeUnknown = 0 };
    virtual ~ISceneNode();
    virtual void BeginUpdate(float);
    virtual void UpdateAI(float);
    virtual void CommitAI();
    virtual void AttemptUpdate(float);
    virtual void UpdateCollisionVolumes();
    virtual void OnCollision(const CCollision &);
    virtual int Constrain(float);
    virtual void CommitUpdate();
    virtual void PostUpdate(float);
    virtual void Draw(CDrawContext &);
    virtual int Attach(ISceneNode &, CMatrix *, unsigned int);
    virtual int Detach();
    virtual int GetLocalBoundingVolume(ISceneNode::EVolumeType) const;
    virtual int GetWorldBoundingVolume(ISceneNode::EVolumeType) const;
    virtual void GetTMLocalToWorld(CMatrix &) const;
    virtual CVector3 GetWorldLinearVelocity() const;
    virtual void GetPosition(CVector3 &) const;
    virtual void GetCentroid(CVector3 &) const;
    virtual void GetRightward(CVector3 &) const;
    virtual void GetForward(CVector3 &) const;
    virtual void GetUpward(CVector3 &) const;
    virtual int IsVisible(CDrawContext &) const;
    virtual unsigned int IsDrawEnabled() const;
    virtual void SetDrawEnabled(bool);
    virtual EClsnId GetCollisionId() const;
    virtual void SetCollisionId(EClsnId);
    virtual int GetScriptObject() const;
    virtual void TriggerScriptEvent(int, void *, bool);
    virtual void HandleBulletCollision(CBullet *, const CCollision &);
    virtual IMovingSceneNode *AsMovingNode();
    virtual const IMovingSceneNode *AsMovingNode() const;
    virtual CStaticObject *AsStaticObject();
    virtual const CStaticObject *AsStaticObject() const;
    virtual CHierObject *AsHierObject();
    virtual const CHierObject *AsHierObject() const;
    virtual CWorldObject *AsWorldObject();
    virtual const CWorldObject *AsWorldObject() const;
    virtual CAnimObject *AsAnimObject();
    virtual const CAnimObject *AsAnimObject() const;
    virtual CSoldierObject *AsSoldierObject();
    virtual const CSoldierObject *AsSoldierObject() const;
    virtual CPlayerObject *AsPlayerObject();
    virtual const CPlayerObject *AsPlayerObject() const;
    virtual CAnimatedPlayer *AsAnimatedPlayerObject();
    virtual const CAnimatedPlayer *AsAnimatedPlayerObject() const;
    virtual CPlayerWeaponObject *AsPlayerWeaponObject();
    virtual const CPlayerWeaponObject *AsPlayerWeaponObject() const;
    virtual CLight *AsLight();
    virtual const CLight *AsLight() const;
    virtual CProjector *AsProjector();
    virtual const CProjector *AsProjector() const;
    virtual CBullet *AsBullet();
    virtual const CBullet *AsBullet() const;
    virtual CCollisionVolume *AsCollisionVolume();
    virtual const CCollisionVolume *AsCollisionVolume() const;
    virtual CBotObject *AsBotObject();
    virtual const CBotObject *AsBotObject() const;
    virtual CAIDoodad *GetAIDoodad();
    virtual const CAIDoodad *GetAIDoodad() const;
    virtual CAIObject *GetAIObject();
    virtual void SetAttachedLight(CLight *, const CVector3 &);
    virtual CLight *GetAttachedLight() const;
    virtual void SerializeSaveData();
};

// Virtual slots 68-88, introduced after ISceneNode. No IMovingSceneNode table is
// emitted; the order comes from derived tables such as IParticleSystem's.
class IMovingSceneNode : public ISceneNode {
  public:
    IMovingSceneNode *AsMovingNode();
    const IMovingSceneNode *AsMovingNode() const;
    virtual void Reset();
    virtual void Halt();
    virtual void ApplyForceTo(const CVector3 &, float);
    virtual void SetTMLocalToWorld(const CMatrix &);
    virtual void PreTransform(const CMatrix &);
    virtual void Transform(const CMatrix &);
    virtual void Move(const CVector3 &);
    virtual void Rotate(const CVector3 &, float);
    virtual void Pitch(float);
    virtual void Roll(float);
    virtual void Yaw(float);
    virtual void SetPosition(const CVector3 &);
    virtual void SetBasis(const CVector3 &, const CVector3 &, const CVector3 &);
    virtual void Orthonormalize();
    virtual void EnterLightVolume(BPDLightVolume *);
    virtual void ExitLightVolume(BPDLightVolume *);
    virtual BPDLightVolume *GetLightVolume();
    virtual void SetLightVolumeTransitionDuration(float);
    virtual const char *GetName();
    virtual int GetTeam();
    virtual int GetPlayerIndex();
};

// Member-only view of the scene container; only the world pointer at +0x18 is named.
class CScene {
  public:
    unsigned char unknown00[0x18];
    CWorldObject *world; // +0x18
    void Add(ISceneNode &, ISceneNode *);
    void Remove(ISceneNode &);
    int IsNodeInScene(const ISceneNode *) const;
    CPlayerObject *GetPlayer(int) const;
    CPlayerObject *FindClosestPlayer(const CVector3 &);
};
extern CScene g_scene;

#endif
