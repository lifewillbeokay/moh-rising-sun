// AI-assisted reconstruction of source/Character/LinearBlend.cpp; see docs/Animation.md.
#include "profile.h"
#include "CharacterTasks.h"
namespace EALA {
namespace Character {

Resource::Pointer<LinearBlend> LinearBlend::Create(Pointer<State> &state, unsigned int substate, Task *parent,
                                                   const char *name, int count, SubTaskInfo *tasks) {
    return new LinearBlend(state, substate, parent, name, count, tasks);
}

LinearBlend::LinearBlend(Pointer<State> &state, unsigned int substate, Task *parent, const char *name, int count,
                         SubTaskInfo *tasks)
    : Task(state, substate, parent), name(name), count(count) {
    lowIndex = highIndex = -1;
    for (unsigned int i = 0; i < this->count; i++)
        this->tasks[i] = tasks[i];
}

LinearBlend::~LinearBlend() {}

int LinearBlend::Busy() {
    if (lowIndex < 0)
        return 1;
    return tasks[0].task->Busy();
}

void LinearBlend::EvalPose(float time, float *pose) {
    if (!pose)
        return;
    float value = Choreographer::Current()->GetClient()->GetValue(name);
    for (unsigned int i = 0; i < count; i++) {
        if (value > tasks[i].high)
            continue;
        if (value >= tasks[i].low || i == 0) {
            tasks[i].task->Evaluate(time, pose);
            return;
        }
        lowIndex = i - 1;
        highIndex = i;
        PerformBlend(value, time, pose);
        return;
    }
    Choreographer::Current()->GetClient()->GetSkeleton()->SetStillPose(0);
}

} // namespace Character
} // namespace EALA
