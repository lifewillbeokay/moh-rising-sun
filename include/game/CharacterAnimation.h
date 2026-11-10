// AI-assisted reconstruction of EALA::Character::Animation's cached-animation view; see docs/Animation.md.
#ifndef MOH_CHARACTERANIMATION_H
#define MOH_CHARACTERANIMATION_H
extern "C" void *DWI_alloc(const char *where, unsigned int size, unsigned int flags);
#include "EAGLAnim/AnimCore.h"
#include "EAGLAnim/AnimBank.h"
#include "CharacterTask.h"
#include "EAGLDynamicLoader.h"
namespace EALA {
class EAGLLoader : public Resource {
  public:
    static Pointer<EAGLLoader> LoadFromMemory(void *data, unsigned int size, const char *name);
    EAGL::DynamicLoader *dynamicLoader; // +0x08
};
} // namespace EALA
template <class T> T max(T a, T b);
template <class T> T min(T a, T b);
#endif
