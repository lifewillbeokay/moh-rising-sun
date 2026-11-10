// AI-assisted reconstruction of the tasks derived from EALA::Character::Task; see docs/Animation.md.
// Class and method names come from the symbol table; member names are inferred from their use.
#ifndef MOH_CHARACTERTASKS_H
#define MOH_CHARACTERTASKS_H
#include "CharacterTask.h"
namespace EALA {
namespace Character {
// A task that redirects evaluation to another substate of its state.
class Jump : public Task {
  public:
    Jump(Resource::Pointer<State> &state, unsigned int substate, Task *parent);
    virtual ~Jump();
    unsigned int target; // +0x34
};
class Sequence : public Task {
  public:
    Sequence(Resource::Pointer<State> &state, unsigned int substate, Task *parent);
    virtual ~Sequence();
    static Resource::Pointer<Sequence> Create(Resource::Pointer<State> &state, unsigned int substate, Task *parent);
    void AddTask(Resource::Pointer<Task> task);
};
class Transition : public Task {
  public:
    Transition(Task *parent, Resource::Pointer<Task> from, Resource::Pointer<Task> to, float time);
    virtual ~Transition();
    static Resource::Pointer<Task> Create(Task *parent, Resource::Pointer<Task> from, Resource::Pointer<Task> to,
                                          float time);
};
class Sentinel {
  public:
    static Resource::Pointer<Task> Create(Resource::Pointer<State> &state, unsigned int substate, Task *parent);
};
// One step of the task tree that re-evaluates its state's substate as children finish.
class StateIterator : public Task {
  public:
    void *operator new(unsigned int size);
    void operator delete(void *p, unsigned int size);
    static Pointer<StateIterator> Alloc(Pointer<State> &state, unsigned int substate, Task *parent);
    StateIterator(Pointer<State> &state, unsigned int substate, Task *parent);
    virtual ~StateIterator();
    void Set(Pointer<State> &state, unsigned int substate, float time);
    void Set(unsigned int substate, float time);
    virtual void EvalPose(float time, float *pose);
    virtual void Dump(unsigned int depth);
    virtual void HandleMessage();
    virtual void HandleOpcode(Opcode &opcode);
    virtual void HandlePose(unsigned int pose);
    virtual void HandleRotation(float rotation);
    virtual void HandleCycleEnd();
    virtual void HandleAnimDuration(float duration);
    virtual void Promote(Pointer<Task> &task);
    virtual StateIterator *AsIterator();

  private:
    Pointer<Task> child; // +0x34
};
} // namespace Character
} // namespace EALA
#endif
