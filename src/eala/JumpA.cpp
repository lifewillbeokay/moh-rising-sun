// AI-assisted reconstruction of source/Character/Jump.cpp; see docs/Animation.md.
#include "profile.h"
#include "CharacterTasks.h"
namespace EALA {
namespace Character {

Resource::Pointer<Jump> Jump::Create(Pointer<State> &state, unsigned int substate, Task *parent, unsigned int target,
                                     unsigned int blendFrames) {
    return new Jump(state, substate, parent, target, blendFrames);
}

Jump::Jump(Pointer<State> &state, unsigned int substate, Task *parent, unsigned int target, unsigned int blendFrames)
    : Task(state, substate, parent), target(target), blendFrames(blendFrames), evaluated(0) {}

int Jump::Busy() { return 1; }

void Jump::Assign(Pointer<Task> &task) { kept = task; }

} // namespace Character
} // namespace EALA
