// AI-assisted reconstruction of source/Character/StateIterator.cpp; see docs/Animation.md.
#include "profile.h"
#include "CharacterTasks.h"
namespace EALA {
namespace Character {

void StateIterator::Set(Pointer<State> &state, unsigned int substate, float time) {
    SetState(state);
    Set(substate, time);
}

} // namespace Character
} // namespace EALA
