// AI-assisted reconstruction of source/Character/Transition.cpp; see docs/Animation.md.
#include "profile.h"
#include "CharacterTasks.h"
namespace EALA {
namespace Character {

Transition::Transition(Task *parent, Pointer<Task> from, Pointer<Task> to, float time)
    : Task(to->GetState(), to->substate, parent), from(from), to(to), remaining(time), duration(time), forwarding(0) {
    from->Reparent(this);
    to->Reparent(this);
}

int Transition::Busy() { return remaining > 0.0f; }

} // namespace Character
} // namespace EALA
