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

void Playback::Dump(unsigned int depth) {
    DumpPrefix(depth);
    DebugMsg("Playback\n");
}

float Playback::GetAnimDuration() { return duration; }

COORD3 Playback::GetAnimTranslation() {
    COORD3 delta;
    SetDifference(delta, endTranslation, startTranslation);
    return delta;
}

} // namespace Character
} // namespace EALA
