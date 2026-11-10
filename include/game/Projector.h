#ifndef GAME_PROJECTOR_H
#define GAME_PROJECTOR_H
// AI-assisted scoped view of CProjector; see docs/Rendering.md. Method names come from the symbol
// table; member names are inferred from use and only the touched offsets are established.
#include "SceneNode.h"
#include "CMatrix.h"
#include "VolFrustum.h"
class CScene;
class CProjector;
class CProjectorSystemDef;
class CProjector : public IMovingSceneNode {
  public:
    unsigned char unknown_40[64 - sizeof(IMovingSceneNode)];
    CVolFrustum frustum;     // +64
    CMatrix localToWorld;    // +368
    CMatrix projection;      // +432
    CMatrix offset;          // +496; applied after the parent's transform.
    CMatrix *attachMatrix;   // +560
    float radius;            // +564
    float frustumParams[4];  // +568
    unsigned char unknown_24c[4];
    int type;                // +588
    int enabled250;          // +592: the constructor sets it, InitProjector clears it. Name inferred.
    unsigned char unknown_254[608 - 596];
    virtual ~CProjector();
    static void *operator new(unsigned int, void *);
    static void operator delete(void *);
    static CScene *m_pScene;
    static void *g_MOH3ProjectionTexture;
    CProjector(CProjectorSystemDef &);
    static CProjector *Create(unsigned int, bool);
    static void InitClass(CScene *);
    static void ShutdownClass();
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
    void UpdateProjection();
    void BeginUpdate(float);
    void Draw(CDrawContext &);
    int Detach();
    int IsVisible(CDrawContext &) const;
};
// A pool element: Create constructs a 608-byte CProjector, so the link at +608 lies outside it and the
// stride is 624. The allocation's tag names source/system/freepool.h; this shape is inferred.
struct CProjectorPoolNode {
    unsigned char object[sizeof(CProjector)];
    CProjectorPoolNode *next;
    unsigned char unknown_264[12];
};
// The global pool, with the same shape as the particle system pool.
struct ProjectorList {
    CProjectorPoolNode *storage;
    CProjectorPoolNode *active;
    CProjectorPoolNode *free;
    int capacity;
    int count;
};
extern ProjectorList g_ProjectorList;
#endif
