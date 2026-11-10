// AI-assisted reconstruction of source/Character/Turn.cpp; see docs/Animation.md.
#include "profile.h"
#include "CharacterTasks.h"
namespace EALA {
namespace Character {

Resource::Pointer<Turn> Turn::Create(Pointer<State> &state, unsigned int substate, Task *parent) {
    return new Turn(state, substate, parent);
}

int Turn::Busy() {
    if (!(Task *)task)
        return 0;
    int busy;
    if (started)
        busy = task->Busy();
    else
        busy = 1;
    return busy;
}

void Turn::EvalPose(float time, float *pose) {
    if (pose) {
        started = 1;
        task->Evaluate(time, pose);
        float rotation = Choreographer::Current()->GetClient()->GetRotation();
        rotation += rate * time;
        parent->HandleRotation(rotation);
    }
}

void Turn::Dump(unsigned int depth) {
    DumpPrefix(depth);
    DebugMsg("Turn\n");
}

void Turn::Assign(Pointer<Task> &newTask, float to, float from, float duration) {
    task = newTask;
    float delta = to - from;
    float lower = -3.1415927f;
    float upper = 3.1415927f;
    if (!(delta <= upper)) {
        do {
            delta -= 6.2831855f;
        } while (delta > upper);
    }
    if (!(delta >= lower)) {
        do {
            delta += 6.2831855f;
        } while (delta < lower);
    }
    rate = delta;
    if (!(duration <= 0.001f))
        rate = delta / duration;
}

} // namespace Character
} // namespace EALA
