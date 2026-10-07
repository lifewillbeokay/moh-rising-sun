// AI-assisted scoped reconstruction from GR8E69; see docs/Bullets.md.
#ifndef GAME_BULLET_H
#define GAME_BULLET_H
#pragma interface
#include "CMatrix.h"
#include "SceneNode.h"

class CCollision;
class CProjectileBullet;
class CThrownBullet;

// CBullet's table (0x802e9eb0) follows the IMovingSceneNode slot map and adds slots
// 89-102 in this order. Field names are descriptive; unlisted storage is unknown.
class CBullet : public IMovingSceneNode {
  public:
    enum EXPLOSION_PARTICLE_TYPE { ExplosionParticleTypeUnknown = 0 };

    unsigned char unknown_18[0x40 - 0x18];
    EClsnId collisionId;
    unsigned char unknown_44[0x70 - 0x44];
    CMatrix localToWorld;
    float field_b0, field_b4;

    void Draw(CDrawContext &);
    unsigned int IsDrawEnabled() const;
    void GetPosition(CVector3 &) const;
    void GetRightward(CVector3 &) const;
    void GetForward(CVector3 &) const;
    void GetTMLocalToWorld(CMatrix &) const;
    CBullet *AsBullet();
    const CBullet *AsBullet() const;

    virtual void Init(bool);
    virtual void Shutdown();
    virtual void GetUp(CVector3 &) const;
    virtual float GetDamage() const;
    virtual int GetScriptBulletType() const;
    virtual ISceneNode *GetFiredBy() const;
    virtual CThrownBullet *AsThrown();
    virtual CProjectileBullet *AsProjectile();
    virtual void GetVelocity(CVector3 &);
    virtual void SetExclusionPair(ISceneNode *);
    virtual void Penetrate(const CCollision &);
    virtual void SetDamage(float);
    virtual void SetBlastRadius(float);
    virtual void SetExplosionParticleSystem(unsigned long, EXPLOSION_PARTICLE_TYPE);
};

// Scoped record views: only the fields the accessors read are named.
struct ProjectileBulletProperties_struct {
    unsigned char unknown_00[76];
    float damage;
};
struct ThrownBulletProperties_struct {
    unsigned char unknown_00[92];
    float damage;
    float blastRadius;
};

// Bit 28 (counting from the least significant bit) of the word at +0x10c.
struct ProjectileBulletFlags {
    unsigned int unknown_31_29 : 3;
    unsigned int drawForward : 1;
    unsigned int unknown_low : 28;
};

class CProjectileBullet : public CBullet {
  public:
    unsigned char unknown_b8[0xc8 - 0xb8];
    ProjectileBulletProperties_struct *properties; // Stored by Init.
    unsigned char unknown_cc[0xe0 - 0xcc];
    ISceneNode *firedBy;                            // Returned by GetFiredBy.
    unsigned char unknown_e4[0x10c - 0xe4];
    ProjectileBulletFlags flags;

    EClsnId GetCollisionId() const;
    float GetDamage() const;
    ISceneNode *GetFiredBy() const;
    CProjectileBullet *AsProjectile();
    void SetExclusionPair(ISceneNode *);
    void SetDamage(float);
    virtual void SetDrawForward(bool);
};

class CThrownBullet : public CBullet {
  public:
    unsigned char unknown_b8[0xc8 - 0xb8];
    ThrownBulletProperties_struct *properties;     // Stored by Init.
    unsigned char unknown_cc[0xe0 - 0xcc];
    ISceneNode *firedBy;                            // Returned by GetFiredBy.
    unsigned char unknown_e4[0x254 - 0xe4];
    CLight *attachedLight;
    unsigned char unknown_258[0x264 - 0x258];
    // Four observed slots at +0x264..+0x270, initialized by CThrownBullet::Init.
    // This remains a scoped view, not a complete object allocation.
    unsigned long explosionParticleSystems[4];

    EClsnId GetCollisionId() const;
    float GetDamage() const;
    ISceneNode *GetFiredBy() const;
    CThrownBullet *AsThrown();
    void SetDamage(float);
    void SetBlastRadius(float);
    void SetExplosionParticleSystem(unsigned long, EXPLOSION_PARTICLE_TYPE);
    CLight *GetAttachedLight() const;
    virtual unsigned int IsCollisionEnabled() const;
};

#endif
