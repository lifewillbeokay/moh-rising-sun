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
namespace EAGLAnim {
class AnimBank;
}
namespace EALA {
namespace Character {
class Jump;
class Task : public Resource, public ISubject {
  public:
    Task(State *state, unsigned int substate, Task *parent);
    virtual ~Task();
    virtual int Busy();
    virtual void Evaluate(float time, float *pose);
    virtual void EvalPose(float time, float *pose);
    virtual float GetAnimDuration();
    virtual COORD3 GetAnimTranslation();
    virtual float GetTotalRootZAxisRotation();
    virtual void Dump(unsigned int depth) = 0;
    virtual void HandleMessage();
    virtual void HandleOpcode(Opcode &opcode);
    virtual void HandlePose(unsigned int pose);
    virtual void HandleRotation(float rotation);
    virtual void HandleCycleEnd();
    virtual void HandleAnimDuration(float duration);
    virtual void Promote(Pointer<Task> &task);
    virtual class StateIterator *AsIterator();
    virtual class Message *AsMessage();
    virtual class Jump *AsJump();

    State *GetState() const;
    void Reparent(Task *parent);
    void SetState(State *state);
    void SetSubstate(unsigned int substate);
    void SetCorePose(unsigned int pose);
    unsigned int GetCorePose() const;
    StateIterator *ParentIterator();
    void DumpPrefix(unsigned int depth);

    Pointer<State> state;        // +0x10
    unsigned int substate;       // +0x14
    WeakPtr<Task, 8> parent;     // +0x18
    unsigned int corePose;       // +0x30
};
// One entry of a state's substate table. Member names are inferred.
class Operation {
  public:
    virtual void Serialize(Serialize::Any &stream) = 0;
    virtual unsigned int SizeOf() const = 0;
    virtual const char *ClassName() const = 0;
    virtual ~Operation();
    virtual Resource::Pointer<Task> Eval(Resource::Pointer<State> &state, unsigned int substate, Task *parent) = 0;
    virtual unsigned int GetDefaultPose();
    virtual bool IsAuthoredTransition(unsigned int fromPose, unsigned int toPose);
};
struct Substate {
    unsigned int unknown_00;
    Operation *operation; // +0x04
    const char *name;     // +0x08
};
class Animation : public Resource {
  public:
    static Pointer<Animation> Alloc(unsigned int index, EAGLAnim::AnimBank *bank);
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

    unsigned int unknown_08;
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
