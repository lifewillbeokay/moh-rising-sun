// AI-assisted reconstruction of the character animation task base; see docs/Animation.md.
// The destructor stays out of line here so that its definition, inline in CharacterTasks.h, is the key function.
#ifndef MOH_CHARACTERTASKBASE_H
#define MOH_CHARACTERTASKBASE_H
#include "EALAResource.h"
#include "TargetTypes.h"
struct COORD3;
namespace EALA {
namespace Character {
class Opcode;
class State;
class StateIterator;
class Message;
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
    virtual StateIterator *AsIterator();
    virtual Message *AsMessage();
    virtual Jump *AsJump();

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
} // namespace Character
} // namespace EALA
#endif
