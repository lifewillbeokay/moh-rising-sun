// AI-assisted reconstruction of source/Character/Playback.cpp; see docs/Animation.md.
#include "profile.h"
#include "CharacterTasks.h"
extern "C" float fmodf(float, float);
extern "C" float floorf(float);
namespace EALA {
namespace Character {

static inline void SetDifference(COORD3 &result, const COORD3 &a, const COORD3 &b) {
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
}

void Playback::HandleOpcodes() {
    if (opcodeCount == 0)
        return;
    float now = time;
    if (lastOpcodeTime == now && opcodesStarted)
        return;
    float epsilon = 0.001f;
    float lastPhase = fmodf(lastOpcodeTime + epsilon, cycleDuration);
    float phase = fmodf(now + epsilon, cycleDuration);
    lastPhase = floorf(lastPhase * Animation::ms_fPlaybackRate);
    phase = floorf(phase * Animation::ms_fPlaybackRate);
    int from = (int)lastPhase;
    int to = (int)phase;
    if (to == from && opcodesStarted)
        return;
    if ((unsigned int)from < (unsigned int)to) {
        for (unsigned int i = 0; i < opcodeCount; i++) {
            unsigned int opcodeFrame = opcodes[i].frame;
            if ((opcodeFrame > (unsigned int)from || !opcodesStarted) && opcodeFrame <= (unsigned int)to) {
                CheckForPoseChange(opcodes[i]);
                parent->HandleOpcode(opcodes[i]);
            }
        }
    } else {
        for (unsigned int i = 0; i < opcodeCount; i++) {
            if (opcodes[i].frame > (unsigned int)from || !opcodesStarted) {
                CheckForPoseChange(opcodes[i]);
                parent->HandleOpcode(opcodes[i]);
            }
        }
        if (!(now >= duration) && loops > 1) {
            for (unsigned int i = 0; i < opcodeCount; i++) {
                if (opcodes[i].frame <= (unsigned int)to) {
                    CheckForPoseChange(opcodes[i]);
                    parent->HandleOpcode(opcodes[i]);
                }
            }
        }
    }
    lastOpcodeTime = now;
    opcodesStarted = 1;
}

void Playback::CheckForPoseChange(Opcode &opcode) {
    if (opcode.kind == 51217) {
        SetCorePose(opcode.argument);
        parent->HandlePose(GetCorePose());
    }
}

} // namespace Character
} // namespace EALA
