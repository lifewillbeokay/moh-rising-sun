// AI-assisted reconstruction of the character animation task tree; see docs/Animation.md.
// Class and method names come from the symbol table; member names are inferred from their use.
#ifndef MOH_CHARACTERTASK_H
#define MOH_CHARACTERTASK_H
#include "ResourceTypes.h"
#include "TargetTypes.h"
#include "EAGLTransform.h"
void DebugMsg(const char *, ...);
namespace Serialize {
class Any;
}
// Serialized objects restore their tables through a NoInit constructor, so they have no constructing base.
class Serializable {
  public:
    class NoInit {};
    virtual void Serialize(Serialize::Any &stream) = 0;
    virtual unsigned int SizeOf() const = 0;
    virtual const char *ClassName() const = 0;
    void operator delete(void *p);
};
namespace EAGLAnim {
class AnimBank;
class FnAnimMemoryMap;
}
namespace EALA {
namespace Character {
// The serialized object tree a state's substate table points at. Member names are inferred.
class Operation : public Serializable {
  public:
    class Operand;
    class Operator;
    virtual ~Operation() {}
    virtual Resource::Pointer<Task> Eval(Resource::Pointer<State> &state, unsigned int substate, Task *parent) = 0;
    virtual unsigned int GetDefaultPose();
    virtual bool IsAuthoredTransition(unsigned int fromPose, unsigned int toPose);
};
// Inferred: a length-prefixed string held in the file image; the stream relocates or restores the text.
struct SerializedString {
    char *text;
    int length;
};
struct Substate {
    unsigned int unknown_00;
    Operation *operation;  // +0x04
    SerializedString name; // +0x08
};
// A cached handle on one animation of a bank, on a list of all live handles.
class Animation : public Resource {
  public:
    Animation(EAGLAnim::AnimBank *bank, unsigned int index);
    virtual ~Animation();
    static Pointer<Animation> Alloc(unsigned int index, EAGLAnim::AnimBank *bank);
    static void Shutdown();
    static void EncacheAll();
    static void DecacheAll();
    void Encache();
    void Decache();
    float GetDuration();
    void GetRootTransform(float *pose, Skeleton *skeleton, float time, COORD3 &translation, COORD4 &rotation);
    void GetRootTransform(float *pose, Skeleton *skeleton, COORD3 &translation, COORD4 &rotation);
    void SetRootTransform(float *pose, Skeleton *skeleton, const COORD3 &translation, const COORD4 &rotation);
    void EvalSQT(Skeleton *skeleton, float time, float *pose, const EAGLAnim::BoneMask *mask);
    static float ms_fPlaybackRate;
    static Animation *m_pHead;
    EAGLAnim::AnimBank *bank;      // +0x08
    unsigned int index;            // +0x0c
    EAGLAnim::FnAnimMemoryMap *fnAnim; // +0x10
    Animation *previous;           // +0x14
    Animation *next;               // +0x18
};
class State : public Resource {
  public:
    typedef _STL::map<_STL::string, unsigned int> NameMap;
    State();
    virtual ~State();
    void Init();
    void *operator new(unsigned int size, void *where);
    void operator delete(void *);
    static unsigned int SizeOf(unsigned int substates);
    static void UnloadAll();
    Resource::Pointer<Animation> Cache(unsigned int index);
    Resource::Pointer<Task> EvalSubstate(unsigned int substate, Task *parent);
    unsigned int GetSubstateIndex(const char *name);
    Resource::Pointer<Task> FindAuthoredTransition(Task *task, unsigned int fromPose, unsigned int toPose);

    unsigned char version[4];             // +0x08; name inferred, the stream reads four bytes.
    Resource::Pointer<EAGLLoader> loader; // +0x0c
    EAGLAnim::AnimBank *bank;             // +0x10; the symbol Init resolves from the loader.
    NameMap *names;                       // +0x14
    void *fileData;                       // +0x18
    unsigned int fileSize;                // +0x1c
    unsigned int unknown_20;
    unsigned int substateCount;           // +0x24
    Substate *substates[1];               // +0x28; allocated by SizeOf for the real count.
};
} // namespace Character
} // namespace EALA
#endif
