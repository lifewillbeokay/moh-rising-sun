// AI-assisted scoped reconstruction from GR8E69; see docs/Collision.md. Only the members
// these fragments read are named; the offsets come from the code and the names are
// descriptive.
#ifndef GAME_COMPARTMENT_H
#define GAME_COMPARTMENT_H
#include "CDBObject.h"
#include "SceneNode.h"

class CDrawContext;
class CPTHeader;
class CRtcFile;

class CExplosion;
class AnimShape;
namespace EAGL {
class Model;
}
class CCompartment : public ISceneNode {
  public:
    class CInstanceModel;
    virtual ~CCompartment();
    void Init(int, char *, CPTHeader *, CDB *, CRtcFile *);
    void Init(CRtcFile *);
    void InitCDB(int, char *, CDB *);
    static void GlobalReset();
    void DrawShadows(CDrawContext &);
    void HandleExplosion(const CExplosion &);
    static CCompartment *GetShared();
    int DoTextureSwap(int);
    void BeginUpdate(float);
    void DeleteInstanceLoaders();
    int GetInstanceType(int);
    EAGL::Model *GetInstanceModel(int, int);
    void EnableOrDisablePrimers(int);
    virtual unsigned int IsDrawEnabled() const;
    virtual int GetLocalBoundingVolume(ISceneNode::EVolumeType) const;
    virtual int GetWorldBoundingVolume(ISceneNode::EVolumeType) const;
    unsigned char unknown_after_node[64 - sizeof(ISceneNode)];
    CCDBObject collisionVolume;
    CCDBObject detailVolume;
    int index;
    char *name;
    CPTHeader *primerTable;
    CDB *database;
    unsigned char unknown_b0[4];
    unsigned int drawEnabled;
    unsigned char unknown_b8[0xfc - 0xb8];
    // The symbol table shows these are _STL::vector; this view keeps that header, whose inlines
    // own .rodata strings, out of the fragments that do not reach the members.
    template <class T> struct VectorView {
        T *start, *finish;
        unsigned char unknown08[8];
        T *begin() const { return start; }
        T *end() const { return finish; }
    };
    VectorView<CInstanceModel> instanceModels; // +0xfc
    VectorView<AnimShape> animShapes;          // +0x10c; updated each frame.
    VectorView<AnimShape> swapShapes;          // +0x11c; stepped by DoTextureSwap.
};

#endif
