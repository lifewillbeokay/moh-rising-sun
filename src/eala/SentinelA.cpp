// AI-assisted reconstruction of source/Character/Sentinel.cpp; see docs/Animation.md.
#include "profile.h"
#include "CharacterTasks.h"
namespace EALA {
namespace Character {

Resource::Pointer<Task> Sentinel::Create(Pointer<State> &state, unsigned int substate, Task *parent) {
    return new Sentinel(state, substate, parent);
}

Sentinel::Sentinel(Pointer<State> &state, unsigned int substate, Task *parent) : Task(state, substate, parent) {}

} // namespace Character
} // namespace EALA
