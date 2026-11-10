// AI-assisted reconstruction of source/Character/Sequence.cpp; see docs/Animation.md.
#include "profile.h"
#include "CharacterTasks.h"
namespace EALA {
namespace Character {

Sequence::Sequence(Pointer<State> &state, unsigned int substate, Task *parent) : Task(state, substate, parent), count(0) {}

} // namespace Character
} // namespace EALA
