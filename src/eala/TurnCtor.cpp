// AI-assisted reconstruction of source/Character/Turn.cpp; see docs/Animation.md.
#include "profile.h"
#include "CharacterTasks.h"
namespace EALA {
namespace Character {

Turn::Turn(Pointer<State> &state, unsigned int substate, Task *parent)
    : Task(state, substate, parent), started(0), unknown_44(0.0f) {}

} // namespace Character
} // namespace EALA
