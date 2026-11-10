#ifndef GAME_DECAL_MANAGER_H
#define GAME_DECAL_MANAGER_H
#pragma interface
// AI-assisted scoped view of CDecalManager; see docs/Rendering.md. Method names come from the symbol
// table. Member names are inferred from use and only the touched offsets are established.
#include "SceneNode.h"
class CCollision;
class CDrawContext;
struct SClsnContact;
class Decal;
class CDecalManager : public ISceneNode {
  public:
    unsigned char unknown_18[60 - 24];
    int decalCount; // +60: live decals across all sets; AcquireDecal recycles at 256.
    // +64: an _STL::vector<DecalSet>, held as storage so this header needs no STL.
    unsigned char sets[12];
    unsigned char unknown_4c[4];
    static CDecalManager *GetSingleton();
    static void ClearDecals();
    static void Reset();
    static void AddBulletDecal(const CCollision &, bool);
    Decal *AcquireDecal(int);
    void InitDecalFromCollision(Decal &, const SClsnContact *, bool);
    int IsVisible(CDrawContext &) const;
    unsigned int IsDrawEnabled() const;
    void Draw(CDrawContext &);
    static void ExecuteDraw(CDrawContext &, void **);
};
#endif
