// AI-assisted reconstruction of source/Character/State.cpp; see docs/Animation.md.
#include "profile.h"
#include "CharacterAnimation.h"
inline void *operator new(unsigned int size, const char *label) { return DWI_alloc(label, size, 1024); }
namespace EALA {
namespace Character {

// Builds the name table and, when the state carries an animation bank in its file image, loads it.
void State::Init() {
    names = new ("source/Character/State.cpp:71") NameMap;
    if (fileData) {
        loader = EAGLLoader::LoadFromMemory(fileData, fileSize, 0);
        EAGL::DynamicLoader::Symbol symbol;
        int iterator = 0;
        EAGL::DynamicLoader *dynamicLoader = loader->dynamicLoader;
        dynamicLoader->GetNextSymbol(EAGL::DynamicLoader::AnimBankType, iterator, symbol);
        dynamicLoader->Release();
        bank = (EAGLAnim::AnimBank *)symbol.data;
    }
    for (unsigned int i = 0; i < substateCount; i++) {
        if (substates[i]->name.text)
            (*names)[_STL::string(substates[i]->name.text)] = i;
    }
}

} // namespace Character
} // namespace EALA
