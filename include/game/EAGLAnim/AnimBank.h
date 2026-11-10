// Attributed reference names and target-verified storage; see docs/Animation.md and src/eagl_anim/NOTICE.
#ifndef MOH_ANIMBANK_H
#define MOH_ANIMBANK_H
#pragma interface
#include "AnimCore.h"
namespace EAGL {
class DynamicLoader;
}
namespace EAGLAnim {
// Only the prefix accessed by these functions is modeled. No complete size is claimed.
class AnimBank {
  public:
    int GetNumAnims() const { return mNumAnims; }
    AnimMemoryMap *GetAnim(int i) const { return mAnims[i]; }
    const char *GetAnimName(int i) const { return mAnimNames[i]; } // Name inferred: "S_0x%08x" hashes per Animation.
    static FnAnimMemoryMap *NewFnAnim(AnimMemoryMap *);
    FnAnimMemoryMap *NewFnAnim(int) const;
    static void Constructor(void *, EAGL::DynamicLoader *, const char *);
    static void Destructor(void *);

  private:
    unsigned int unknown_0;
    int mNumAnims;
    unsigned int unknown_8;
    unsigned int unknown_c;
    AnimMemoryMap **mAnims;
    const char **mAnimNames;
};
} // namespace EAGLAnim
#endif
