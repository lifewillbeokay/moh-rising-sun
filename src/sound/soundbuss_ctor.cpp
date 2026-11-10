// AI-assisted reconstruction of the sound buss; see docs/Sound.md.
#include "SoundBuss.h"
cSoundBuss::cSoundBuss() {
    active = 0;
    volume = 1.0f;
    currentEvent = 0;
    eventCount = 0;
}
