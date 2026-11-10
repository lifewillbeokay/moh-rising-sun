// AI-assisted reconstruction of source/Character/Animation.cpp; see docs/Animation.md.
#include "profile.h"
#include "CharacterAnimation.h"
extern "C" void *bsearch(const void *, const void *, unsigned int, unsigned int, int (*)(const void *, const void *));
extern "C" int sscanf(const char *, const char *, ...);
extern "C" void qsort(void *, unsigned int, unsigned int, int (*)(const void *, const void *));
void *TLT_GetAnimationBank();
int TLT_GetAnimationBankSize();
// One entry of the sorted table from an animation's name hash to its index in the bank. The name is the symbol's.
struct AnimID {
    unsigned int hash;
    unsigned int index;
    void Set(const char *name, unsigned int index) {
        this->index = index;
        sscanf(name, "S_0x%08x", &hash);
    }
    static int Compare(const void *, const void *);
};
inline void *operator new(unsigned int size, const char *label) { return DWI_alloc(label, size, 1024); }
extern EALA::Resource::Pointer<EALA::EAGLLoader> g_hLoader;
extern AnimID g_vBankMap[];
extern unsigned int g_uBankMapSize;
extern EAGLAnim::AnimBank *g_pBank;
namespace EALA {
namespace Character {


void Animation::Shutdown() {
    g_pBank = 0;
    g_hLoader = 0;
    g_uBankMapSize = 0;
}

} // namespace Character
} // namespace EALA

// Loads the level's animation bank and sorts a table from each animation's name hash to its index.
void Gameloop_LoadLevel_Fixup_Animation() {
    using namespace EALA;
    using namespace EALA::Character;
    void *bank = TLT_GetAnimationBank();
    g_hLoader = EAGLLoader::LoadFromMemory(bank, TLT_GetAnimationBankSize(), 0);
    EAGL::DynamicLoader::Symbol symbol;
    int iterator = 0;
    EAGL::DynamicLoader *loader = g_hLoader->dynamicLoader;
    loader->GetNextSymbol(EAGL::DynamicLoader::AnimBankType, iterator, symbol);
    loader->Release();
    g_pBank = (EAGLAnim::AnimBank *)symbol.data;
    unsigned int count = g_pBank->GetNumAnims();
    for (unsigned int i = 0; i < count; i++) {
        g_vBankMap[g_uBankMapSize].Set(g_pBank->GetAnimName(i), i);
        g_uBankMapSize++;
    }
    qsort(g_vBankMap, g_uBankMapSize, sizeof(AnimID), AnimID::Compare);
}

namespace EALA {
namespace Character {

Resource::Pointer<Animation> Animation::Alloc(unsigned int index, EAGLAnim::AnimBank *bank) {
    if (bank)
        return new ("source/Character/Animation.cpp:192") Animation(bank, index);
    AnimID key = {0, 0};
    key.hash = index;
    AnimID *entry = (AnimID *)bsearch(&key, g_vBankMap, g_uBankMapSize, sizeof(AnimID), AnimID::Compare);
    if (entry)
        return new ("source/Character/Animation.cpp:212") Animation(g_pBank, entry->index);
    return 0;
}

Animation::Animation(EAGLAnim::AnimBank *bank, unsigned int index) : bank(bank), index(index) {
    Encache();
    previous = 0;
    next = m_pHead;
    if (next)
        next->previous = this;
    m_pHead = this;
}

Animation::~Animation() {
    Decache();
    if (previous)
        previous->next = next;
    if (next)
        next->previous = previous;
    if (m_pHead == this)
        m_pHead = next;
}

void Animation::Encache() { fnAnim = bank->NewFnAnim(index); }

void Animation::Decache() {
    if (fnAnim) {
        EAGLAnim::MemoryPoolManager::DeleteFnAnim(fnAnim);
        fnAnim = 0;
    }
}

void Animation::EncacheAll() {
    for (Animation *animation = m_pHead; animation; animation = animation->next)
        animation->Encache();
}

void Animation::DecacheAll() {
    for (Animation *animation = m_pHead; animation; animation = animation->next)
        animation->Decache();
}

float Animation::GetDuration() {
    float length = 0.0f;
    fnAnim->GetLength(length);
    float period = 1.0f / ms_fPlaybackRate;
    return max((length - 1.0f) * period, period * 0.5f);
}

} // namespace Character
} // namespace EALA
