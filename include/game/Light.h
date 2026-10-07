// AI-assisted scoped reconstruction from GR8E69; see docs/LightVolumes.md.
#ifndef GAME_LIGHT_H
#define GAME_LIGHT_H
#pragma interface
#include "CMatrix.h"
#include "SceneNode.h"
#include "BPD.h"

// Four floats: 0-255 color components and a fourth word. Assignment copies the
// four floats individually, as CPropertyAnimLight::BeginUpdate's copy shows. The
// type and member names are descriptive.
class LightColor {
  public:
    float r, g, b, a;
    LightColor() {}
    LightColor(float ar, float ag, float ab, float aa) : r(ar), g(ag), b(ab), a(aa) {}
    LightColor &operator=(const LightColor &o) {
        r = o.r;
        g = o.g;
        b = o.b;
        a = o.a;
        return *this;
    }
};

// CLight's table at 0x802e8f48 follows the IMovingSceneNode slot map and adds
// GetPropertyID at slot 89. Field names are descriptive; storage between the
// IObserver prefix and the matrix, and after the radius, is not established.
class CLight : public IMovingSceneNode {
  public:
    unsigned char unknown_18[0x40 - 0x18];
    CMatrix localToWorld;
    CMatrix attachTransform;  // Multiplied by the parent's world matrix in BeginUpdate.
    ISceneNode *parent;       // Cleared by Detach.
    unsigned int attachSlot;  // Attach's third argument.
    LightColor color;
    float field_d8;
    float radius;

    CLight();
    virtual ~CLight();
    static BPDLightVolume *GetDefaultLightVolume();
    static void Register(void *, int);
    void MarkForDestruction(int);
    void Destroy();
    void AttemptUpdate(float);
    void CommitUpdate();
    void Reset();
    void PreTransform(const CMatrix &);
    void Transform(const CMatrix &);
    void SetTMLocalToWorld(const CMatrix &);
    void SetPosition(const CVector3 &);
    void SetBasis(const CVector3 &, const CVector3 &, const CVector3 &);
    void Move(const CVector3 &);
    int Attach(ISceneNode &, CMatrix *, unsigned int);
    int Detach();
    void Rotate(const CVector3 &, float);
    void Orthonormalize();
    void GetTMLocalToWorld(CMatrix &) const;
    void GetPosition(CVector3 &) const;
    void GetRightward(CVector3 &) const;
    void GetForward(CVector3 &) const;
    void GetUpward(CVector3 &) const;
    int IsVisible(CDrawContext &) const;
    CLight *AsLight();
    const CLight *AsLight() const;
    virtual int GetPropertyID();
};

class CPropertyAnimLight : public CLight {
  public:
    MOH_animatedLight_Struct *record;
    short frame;
    unsigned int reverse : 1; // Set while a back-and-forth light runs backward.
    unsigned char unknown_e8[8];
    CPropertyAnimLight *next; // Pool list link used by CAnimLightManager::Destroy.

    CPropertyAnimLight(MOH_animatedLight_Struct *);
    ~CPropertyAnimLight();
    void InitFromProperty();
    void Destroy();
    void BeginUpdate(float);
    int GetPropertyID();
};

class CInstancedAnimLight : public CPropertyAnimLight {
  public:
    CInstancedAnimLight(MOH_animatedLight_Struct *);
    void Destroy();
};

// A 20-byte free-list pool with the same shape as g_particleSystemList: storage,
// used head, free head, capacity and count. Names are descriptive.
struct AnimLightPool {
    CPropertyAnimLight *storage;
    CPropertyAnimLight *active;
    CPropertyAnimLight *free;
    int capacity;
    int count;
};

// Scoped view: registered patterns at +0x38/+0x3c, and pools at +0x44
// (instanced lights) and +0x58 (property lights). Base classes remain unknown.
class CAnimLightManager {
  public:
    unsigned char unknown_0[0x38];
    void *patterns;
    int patternCount;
    unsigned char unknown_40[4];
    AnimLightPool instancedLights;
    AnimLightPool propertyLights;
    void Destroy(CInstancedAnimLight *);
    void Destroy(CPropertyAnimLight *);
};

#endif
