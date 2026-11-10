// AI-assisted reconstruction of the sound buss manager's update and lookup; see docs/Sound.md.
#include "SoundManager.h"
#include "SoundBuss.h"

void cSoundBussMan::Update(float dt) {
    dwi::list<cSoundBuss *>::iterator it(0);
    for (it = active.begin(); it != active.end(); it++) {
        cSoundBuss *buss = *it;
        if (buss) {
            short isActive = buss->active;
            if (isActive) {
                buss->IncrementClock(dt);
            } else {
                AddInactiveRetroFit(buss);
                *it = (cSoundBuss *)isActive;
                activeCount--;
            }
        }
    }
}

cSoundBuss *cSoundBussMan::FindBuss(unsigned int crc) {
    for (cSoundBuss *buss = busses; buss; buss = buss->next) {
        if (buss->crc == crc)
            return buss;
    }
    return 0;
}
