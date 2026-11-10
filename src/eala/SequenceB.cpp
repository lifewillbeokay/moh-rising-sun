// AI-assisted reconstruction of source/Character/Sequence.cpp; see docs/Animation.md.
#include "profile.h"
#include "CharacterTasks.h"
namespace EALA {
namespace Character {

Resource::Pointer<Sequence> Sequence::Create(Pointer<State> &state, unsigned int substate, Task *parent) {
    return new Sequence(state, substate, parent);
}

void Sequence::AddTask(Pointer<Task> task) { tasks[count++] = task; }

void Sequence::Pop() {
    Pointer<Task> keep(this);
    AttemptPromotion();
    for (unsigned int i = 0; i < count - 1; i++)
        tasks[i] = tasks[i + 1];
    tasks[--count] = Pointer<Task>();
}

void Sequence::AttemptPromotion() {
    if (count <= 1) {
        last = tasks[0];
        parent->Promote(last);
    }
}

int Sequence::Busy() {
    if (!count)
        return 0;
    if (!tasks[0]->Busy())
        Pop();
    return count != 0;
}

void Sequence::EvalPose(float time, float *pose) {
    if (count)
        tasks[0]->Evaluate(time, pose);
    else if (last.IsValid())
        last->Evaluate(time, pose);
}

void Sequence::Dump(unsigned int depth) {
    DumpPrefix(depth);
    DebugMsg("Sequence\n");
    for (unsigned int i = 0; i < count; i++)
        tasks[i]->Dump(depth + 1);
}

void Sequence::Promote(Pointer<Task> &task) {
    tasks[0] = task;
    tasks[0]->Reparent(this);
    AttemptPromotion();
}

} // namespace Character
} // namespace EALA
