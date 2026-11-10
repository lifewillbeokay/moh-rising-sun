// AI-assisted reconstruction of source/Character/StateIterator.cpp; see docs/Animation.md.
#include "profile.h"
#include "CharacterTasks.h"
namespace EALA {
namespace Character {

void StateIterator::EvalPose(float time, float *pose) {
    Pointer<Task> keep(child);
    for (unsigned int depth = 0; !child->Busy(); depth++) {
        if (depth > 10) {
            Pointer<State> current(GetState());
            child = Sentinel::Create(current, substate, this);
            break;
        }
        child = child->GetState()->EvalSubstate(child->substate, this);
    }
    child->Evaluate(time, pose);
}

void StateIterator::HandleMessage() {
    if (parent.Get())
        parent->HandleMessage();
    else
        Choreographer::Current()->HandleMessage();
}

void StateIterator::HandleOpcode(Opcode &opcode) {
    if (parent.Get())
        parent->HandleOpcode(opcode);
    else
        Choreographer::Current()->HandleOpcode(opcode);
}

void StateIterator::HandlePose(unsigned int pose) {
    SetCorePose(pose);
    if (parent.Get())
        parent->HandlePose(pose);
}

void StateIterator::HandleRotation(float rotation) {
    if (parent.Get())
        parent->HandleRotation(rotation);
    else
        Choreographer::Current()->HandleRotation(rotation);
}

void StateIterator::HandleCycleEnd() {
    if (parent.Get())
        parent->HandleCycleEnd();
    else
        Choreographer::Current()->HandleCycleEnd();
}

void StateIterator::HandleAnimDuration(float duration) {
    if (parent.Get())
        parent->HandleAnimDuration(duration);
    else
        Choreographer::Current()->HandleAnimDuration(duration);
}

void StateIterator::Promote(Pointer<Task> &task) {
    task->Reparent(this);
    child = task;
}

void StateIterator::Dump(unsigned int depth) {
    DumpPrefix(depth);
    DebugMsg("StateIterator\n");
    child->Dump(depth + 1);
}

} // namespace Character
} // namespace EALA
