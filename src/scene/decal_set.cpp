// AI-assisted reconstruction of the decal set's constructors; see docs/Rendering.md.
#include "profile.h"
#include <list>
#include "Toilet.h"
class Decal {
  public:
    unsigned char unknown_0[48];
    float fade;
    void *set;
};
class DecalSet {
  public:
    _STL::list<Decal> decals;
    int unknown_8;
    LevelFileContentsStruct_ *resource;
    DecalSet(_STL::allocator<Decal> &);
    DecalSet(const DecalSet &);
};
DecalSet::DecalSet(_STL::allocator<Decal> &allocator) : decals(allocator), unknown_8(0), resource(0) {
}
DecalSet::DecalSet(const DecalSet &other) : decals(other.decals), unknown_8(other.unknown_8), resource(other.resource) {
}
