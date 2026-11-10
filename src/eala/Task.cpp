// AI-assisted reconstruction of the character task base; see docs/Animation.md. The source file name is unknown.
#include "profile.h"
#include "CharacterTask.h"
namespace EALA {
namespace Character {

Task::Task(State *state, unsigned int substate, Task *parent) : state(state), substate(substate), parent(parent) {
    corePose = 99;
}

State *Task::GetState() const { return state; }

// Keeps the task alive while it evaluates: its children may release the last other reference.
void Task::Evaluate(float time, float *pose) {
    Pointer<Task> keep(this);
    EvalPose(time, pose);
}

void Task::Reparent(Task *parent) { this->parent = parent; }

float Task::GetAnimDuration() { return 0.0f; }

COORD3 Task::GetAnimTranslation() {
    COORD3 zero;
    zero.x = zero.y = zero.z = 0.0f;
    return zero;
}

float Task::GetTotalRootZAxisRotation() { return 0.0f; }

void Task::SetState(State *state) { this->state = state; }

void Task::SetSubstate(unsigned int substate) { this->substate = substate; }

void Task::SetCorePose(unsigned int pose) { corePose = pose; }

unsigned int Task::GetCorePose() const { return corePose; }

StateIterator *Task::ParentIterator() {
    Task *task = parent.Get();
    while (task && !task->AsIterator())
        task = task->parent.Get();
    return task ? task->AsIterator() : 0;
}

void Task::DumpPrefix(unsigned int depth) {
    char prefix[512] = "<!> ";
    unsigned int i;
    for (i = 0; i < (depth + 1) * 4; i++)
        prefix[4 + i] = ' ';
    prefix[i] = 0;
    DebugMsg(prefix);
}

void Task::HandleMessage() { parent->HandleMessage(); }

void Task::HandleOpcode(Opcode &opcode) { parent->HandleOpcode(opcode); }

void Task::HandlePose(unsigned int pose) {
    SetCorePose(pose);
    parent->HandlePose(pose);
}

void Task::HandleRotation(float rotation) { parent->HandleRotation(rotation); }

void Task::HandleCycleEnd() { parent->HandleCycleEnd(); }

void Task::HandleAnimDuration(float duration) { parent->HandleAnimDuration(duration); }

int Task::Busy() { return 0; }

void Task::EvalPose(float, float *) {}

void Task::Promote(Pointer<Task> &) {}

StateIterator *Task::AsIterator() { return 0; }

Message *Task::AsMessage() { return 0; }

Jump *Task::AsJump() { return 0; }

} // namespace Character
} // namespace EALA
