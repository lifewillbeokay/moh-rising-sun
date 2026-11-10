// AI-assisted reconstruction of the sound buss; see docs/Sound.md.
#include "SoundBuss.h"
void cSoundBuss::IncrementClock(float elapsed) {
    if (active && !(clock <= events[currentEvent].time)) {
        currentEvent++;
        if (currentEvent < eventCount) {
            clock = 0.0f;
        } else {
            active = 0;
            volume = events[currentEvent - 1].target;
        }
    }
    clock += elapsed;
}
void cSoundBuss::DeactivateBuss() {
    if (active) {
        active = 0;
        volume = events[currentEvent].target;
    }
}
