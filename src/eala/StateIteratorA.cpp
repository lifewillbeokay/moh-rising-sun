// AI-assisted reconstruction of source/Character/StateIterator.cpp; see docs/Animation.md.
#include "profile.h"
#include "CharacterTasks.h"
namespace EALA {
namespace Character {

Resource::Pointer<StateIterator> StateIterator::Alloc(Pointer<State> &state, unsigned int substate, Task *parent) {
    return new StateIterator(state, substate, parent);
}

StateIterator::StateIterator(Pointer<State> &state, unsigned int substate, Task *parent)
    : Task(state, substate, parent) {
    child = state->EvalSubstate(substate, this);
}

} // namespace Character
} // namespace EALA
