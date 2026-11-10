// FEng matrices and an object's animated data. Names of FEMatrix4 and FEQuaternion are symbols;
// FEObjData's members are inferred from FEngine's render walk.
#ifndef GAME_FENG_MATH_H
#define GAME_FENG_MATH_H
#include "FEScript.h"

class FEMatrix4 {
  public:
    float m[4][4];
    void Identify();
};
struct FEQuaternion {
    float x, y, z, w;
    void GetMatrix(FEMatrix4 *);
};
struct FEObjData {
    unsigned long unknown_0[3];
    long visible;       // +0xc; also the alpha channel of the color at the start of this block.
    FEVector3 pivot;    // +0x10
    FEVector3 position; // +0x1c; CFengInterface::GenerateXformMatrix translates by it.
    FEQuaternion rotation; // +0x28
    FEVector3 scale;    // +0x38
};
void FEMultMatrix(FEMatrix4 *, const FEMatrix4 *, const FEMatrix4 *);
void FEMultMatrix(FEVector3 *, const FEMatrix4 *, const FEVector3 *);

// One entry per drawable object in a package, sorted by depth before rendering.
struct FEObjectListEntry {
    void *object;    // +0x0
    int depthBits;   // +0x4; the depth's float bits.
};
template <int N> class FEObjectSorter {
  public:
    FEObjectSorter() : count(0) {}
    long count;                    // +0x0
    FEObjectListEntry entries[N];  // +0x4
    FEObjectListEntry scratch[N];
    void SortObjects();
};
#endif
