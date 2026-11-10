// AI-assisted reconstruction of source/Character/Transition.cpp; see docs/Animation.md.
#include "profile.h"
#include "CharacterTasks.h"
namespace EALA {
namespace Character {

void Transition::Dump(unsigned int depth) {
    DumpPrefix(depth);
    DebugMsg("Transition (0x%08x)\n", this);
    if (from.IsValid()) {
        DumpPrefix(depth);
        DebugMsg("PREV:\n");
        from->Dump(depth + 1);
    }
    if (to.IsValid()) {
        DumpPrefix(depth);
        DebugMsg("NEXT:\n");
        to->Dump(depth + 1);
    }
}

void Transition::HandleMessage() {
    if (forwarding)
        parent->HandleMessage();
}

void Transition::HandlePose(unsigned int pose) {
    if (forwarding) {
        SetCorePose(pose);
        parent->HandlePose(pose);
    }
}

void Transition::HandleRotation(float rotation) {
    if (forwarding)
        parent->HandleRotation(rotation);
}

void Transition::HandleCycleEnd() {
    if (forwarding)
        parent->HandleCycleEnd();
}

void Transition::HandleAnimDuration(float duration) {
    if (forwarding)
        parent->HandleAnimDuration(duration);
}

void Transition::Promote(Pointer<Task> &task) {
    if (forwarding) {
        to = task;
        to->Reparent(this);
    } else {
        from = task;
        from->Reparent(this);
    }
}

} // namespace Character
} // namespace EALA
