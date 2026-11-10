// AI-assisted reconstruction of source/Character/Playback.cpp; see docs/Animation.md.
#include "profile.h"
#include "CharacterTasks.h"
namespace EALA {
namespace Character {

static inline void SetDifference(COORD3 &result, const COORD3 &a, const COORD3 &b) {
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
}

Playback::~Playback() {}

Resource::Pointer<Playback> Playback::Create(Resource::Pointer<State> &state, unsigned int substate, Task *parent,
                                             unsigned int animation, const char *name, float start, unsigned int loops) {
    return new Playback(state, substate, parent, animation, name, start, loops);
}

void Playback::AssignOpcodes(unsigned int count, Opcode *opcodes) {
    opcodeCount = count;
    this->opcodes = opcodes;
    for (unsigned int i = 0; i < opcodeCount && this->opcodes[i].frame == 0; i++)
        CheckForPoseChange(this->opcodes[i]);
}

int Playback::Busy() {
    return duration == 0.0f || !(time + 0.001f >= duration);
}

} // namespace Character
} // namespace EALA
