// AI-assisted reconstruction of the sound buss; see docs/Sound.md.
#include "SoundBuss.h"
bool cSoundBuss::ActivateBuss() {
    short wasActive = active;
    if (!wasActive) {
        currentEvent = wasActive;
        active = 1;
        clock = 0.0f;
        return true;
    }
    return false;
}
