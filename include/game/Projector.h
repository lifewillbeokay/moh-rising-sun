#ifndef GAME_PROJECTOR_H
#define GAME_PROJECTOR_H
// AI-assisted scoped view of CProjector; see docs/Rendering.md. Method names come from the symbol
// table; member names are inferred from use and only the touched offsets are established.
#include "SceneNode.h"
#include "CMatrix.h"
#include "VolFrustum.h"
class CScene;
class CProjector;
// The global pool, with the same shape as the particle system pool.
struct ProjectorList {
    CProjector *storage;
    CProjector *active;
    CProjector *free;
    int capacity;
    int count;
};
extern ProjectorList g_ProjectorList;
class CProjector : public IMovingSceneNode {
  public:
    unsigned char unknown_40[64 - sizeof(IMovingSceneNode)];
    CVolFrustum frustum;     // +64
    CMatrix localToWorld;    // +368
    CMatrix projection;      // +432
    unsigned char unknown_1f0[564 - 496];
    float radius;            // +564
    float frustumParams[4];  // +568
    unsigned char unknown_24c[4];
    int type;                // +588
    unsigned char unknown_250[608 - 592];
    CProjector *next;        // +608
    virtual ~CProjector();
    static void *operator new(unsigned int, void *);
    static void operator delete(void *);
    static CProjector *Alloc();
    static void Release(CProjector *);
    static int Destroy(CProjector *);
    void HandleEvent(ISubject *, ESubjectEvent);
    int GetType();
    void Destroy();
    unsigned int IsDrawEnabled() const;
    void GetTMLocalToWorld(CMatrix &) const;
    void GetPosition(CVector3 &) const;
    void GetRightward(CVector3 &) const;
    void GetForward(CVector3 &) const;
    void GetUpward(CVector3 &) const;
    void SetTMLocalToWorld(const CMatrix &);
    void SetFrustum(CVolFrustum &);
    void SetMatrix(CMatrix &);
    void SetRadius(float);
    const CVolFrustum &GetFrustum() const;
    CProjector *AsProjector();
    const CProjector *AsProjector() const;
};
#endif
