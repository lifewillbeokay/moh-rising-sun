// AI-assisted reconstruction of the decal manager's clearing and bullet decals; see docs/Rendering.md.
#include "profile.h"
#include <list>
#include <vector>
#include "DecalManager.h"
#include "CDBObject.h"
#include "Toilet.h"
#include "Volume.h"
// One 56-byte decal; only the words AcquireDecal sets are named.
class Decal {
  public:
    unsigned char unknown_0[48];
    float fade;   // +48: inferred, starts at -1.
    void *set;    // +52: the owning set.
};
// A decal list and its level resource, 16 bytes; the vector element.
class DecalSet {
  public:
    _STL::list<Decal> decals;
    int unknown_8;
    LevelFileContentsStruct_ *resource; // +12: the first value is the surface's decal CRC.
};
typedef _STL::vector<DecalSet> DecalSetVector;
static inline DecalSetVector &Sets(CDecalManager *manager) { return *(DecalSetVector *)manager->sets; }
void CDecalManager::ClearDecals() {
    CDecalManager *manager = GetSingleton();
    for (DecalSet *set = Sets(manager).begin(); set != Sets(manager).end(); set++)
        set->decals.clear();
    manager->decalCount = 0;
}
void CDecalManager::Reset() {
    ClearDecals();
    g_scene.Add(*GetSingleton(), 0);
}
struct DecalSurface {
    unsigned char unknown_0[12];
    short side;
    unsigned short surfaceType;
};
void CDecalManager::AddBulletDecal(const CCollision &collision, bool flag) {
    const SClsnContact *contact = collision.contacts;
    if (contact) {
        DecalSurface *surface = (DecalSurface *)contact->feature;
        unsigned int crc = 0;
        if (surface)
            crc = CSurfaceType::GetDecalCRC(surface->surfaceType, surface->side >= 0);
        CDecalManager *manager = GetSingleton();
        Decal *decal = manager->AcquireDecal(crc);
        if (decal)
            manager->InitDecalFromCollision(*decal, contact, flag);
    }
}
