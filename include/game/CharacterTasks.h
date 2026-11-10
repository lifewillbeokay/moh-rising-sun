// AI-assisted reconstruction of the tasks derived from EALA::Character::Task; see docs/Animation.md.
// Class and method names come from the symbol table; member names are inferred from their use.
#ifndef MOH_CHARACTERTASKS_H
#define MOH_CHARACTERTASKS_H
#pragma interface
#include "CharacterTask.h"
namespace EALA {
namespace Character {
inline Task::~Task() {}
// A task that redirects evaluation to another substate of its state.
class Jump : public Task {
  public:
    static Resource::Pointer<Jump> Create(Resource::Pointer<State> &state, unsigned int substate, Task *parent,
                                          unsigned int target, unsigned int blendFrames);
    Jump(Resource::Pointer<State> &state, unsigned int substate, Task *parent, unsigned int target,
         unsigned int blendFrames);
    virtual ~Jump();
    virtual int Busy();
    virtual void EvalPose(float time, float *pose);
    virtual void Dump(unsigned int depth);
    virtual Jump *AsJump();
    void Assign(Resource::Pointer<Task> &task);
    void *operator new(unsigned int size);
    void operator delete(void *p, unsigned int size);
    unsigned int target;           // +0x34
    unsigned int blendFrames;      // +0x38
    int evaluated;                 // +0x3c
    Resource::Pointer<Task> kept;  // +0x40
};
// A task that sends one message to its parent, then holds a kept task or the still pose.
class Message : public Task {
  public:
    static Resource::Pointer<Message> Create(Resource::Pointer<State> &state, unsigned int substate, Task *parent);
    Message(Resource::Pointer<State> &state, unsigned int substate, Task *parent);
    virtual ~Message();
    virtual int Busy();
    virtual void EvalPose(float time, float *pose);
    virtual void Dump(unsigned int depth);
    virtual Message *AsMessage();
    void Assign(Resource::Pointer<Task> &task);
    void *operator new(unsigned int size);
    void operator delete(void *p, unsigned int size);
    int sent;                      // +0x34
    float poseAge;                 // +0x38
    unsigned int unknown_3c;
    Resource::Pointer<Task> kept;  // +0x40
};
// Plays one animation of a state's bank some number of times, with its opcodes and root motion.
class Playback : public Task {
  public:
    void *operator new(unsigned int size);
    void operator delete(void *p, unsigned int size);
    static Resource::Pointer<Playback> Create(Resource::Pointer<State> &state, unsigned int substate, Task *parent,
                                              unsigned int animation, const char *name, float start, unsigned int loops);
    Playback(Resource::Pointer<State> &state, unsigned int substate, Task *parent, unsigned int animation,
             const char *name, float start, unsigned int loops);
    virtual ~Playback();
    virtual int Busy();
    virtual void EvalPose(float time, float *pose);
    virtual float GetAnimDuration();
    virtual COORD3 GetAnimTranslation();
    virtual float GetTotalRootZAxisRotation();
    virtual void Dump(unsigned int depth);
    void AssignOpcodes(unsigned int count, Opcode *opcodes);
    void HandleOpcodes();
    void CheckForPoseChange(Opcode &opcode);
    Resource::Pointer<Animation> animation; // +0x34
    float time;                 // +0x38
    float unknown_3c;           // +0x3c
    float duration;             // +0x40; the whole run, all loops.
    float cycleDuration;        // +0x44
    unsigned int loops;         // +0x48
    unsigned int animationIndex; // +0x4c
    const char *name;           // +0x50
    COORD3 startTranslation;    // +0x54
    COORD3 endTranslation;      // +0x60
    COORD3 lastTranslation;     // +0x6c
    COORD4 startRotation;       // +0x78
    COORD4 endRotation;         // +0x88
    COORD4 lastRotation;        // +0x98
    COORD4 startInverse;        // +0xa8
    COORD4 lastInverse;         // +0xb8
    COORD4 rootDelta;           // +0xc8
    unsigned int opcodeCount;   // +0xd8
    Opcode *opcodes;            // +0xdc
    float lastOpcodeTime;       // +0xe0
    int opcodesStarted;         // +0xe4
};
// Runs up to sixteen tasks in order and promotes the last one into its parent when it is alone.
class Sequence : public Task {
  public:
    static Resource::Pointer<Sequence> Create(Resource::Pointer<State> &state, unsigned int substate, Task *parent);
    Sequence(Resource::Pointer<State> &state, unsigned int substate, Task *parent);
    virtual ~Sequence();
    virtual int Busy();
    virtual void EvalPose(float time, float *pose);
    virtual void Dump(unsigned int depth);
    virtual void Promote(Resource::Pointer<Task> &task);
    void AddTask(Resource::Pointer<Task> task);
    void Pop();
    void AttemptPromotion();
    void *operator new(unsigned int size);
    void operator delete(void *p, unsigned int size);
    Resource::Pointer<Task> tasks[16]; // +0x34
    unsigned int count;                // +0x74
    Resource::Pointer<Task> last;      // +0x78; the task that was promoted.
};
// Plays a task while turning the character at the rate that covers the wrapped angle in the given time.
class Turn : public Task {
  public:
    static Resource::Pointer<Turn> Create(Resource::Pointer<State> &state, unsigned int substate, Task *parent);
    Turn(Resource::Pointer<State> &state, unsigned int substate, Task *parent);
    virtual ~Turn();
    virtual int Busy();
    virtual void EvalPose(float time, float *pose);
    virtual void Dump(unsigned int depth);
    void Assign(Resource::Pointer<Task> &task, float to, float from, float duration);
    void *operator new(unsigned int size);
    void operator delete(void *p, unsigned int size);
    Resource::Pointer<Task> task; // +0x34
    unsigned char unknown_38[8];
    int started;                  // +0x40
    float unknown_44;             // +0x44
    float rate;                   // +0x48
};
// Blends from one task into another over a fixed time, then promotes the target into its parent.
class Transition : public Task {
  public:
    static Resource::Pointer<Task> Create(Task *parent, Resource::Pointer<Task> from, Resource::Pointer<Task> to,
                                          float time);
    Transition(Task *parent, Resource::Pointer<Task> from, Resource::Pointer<Task> to, float time);
    virtual ~Transition();
    virtual int Busy();
    virtual void EvalPose(float time, float *pose);
    virtual void Dump(unsigned int depth);
    virtual void HandleMessage();
    virtual void HandlePose(unsigned int pose);
    virtual void HandleRotation(float rotation);
    virtual void HandleCycleEnd();
    virtual void HandleAnimDuration(float duration);
    virtual void Promote(Resource::Pointer<Task> &task);
    void PerformBlend(float weight, float time, float *pose);
    void *operator new(unsigned int size);
    void operator delete(void *p, unsigned int size);
    Resource::Pointer<Task> from; // +0x34
    Resource::Pointer<Task> to;   // +0x38
    float remaining;              // +0x3c
    float duration;               // +0x40
    int forwarding;               // +0x44; events pass to the parent only while set.
};
// The task a StateIterator installs when no substate settles: it holds the skeleton's still pose.
class Sentinel : public Task {
  public:
    static Resource::Pointer<Task> Create(Resource::Pointer<State> &state, unsigned int substate, Task *parent);
    Sentinel(Resource::Pointer<State> &state, unsigned int substate, Task *parent);
    virtual ~Sentinel();
    virtual int Busy();
    virtual void EvalPose(float time, float *pose);
    virtual void Dump(unsigned int depth);
    void *operator new(unsigned int size);
    void operator delete(void *p, unsigned int size);
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
