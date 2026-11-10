// AI-assisted reconstruction of the sound buss manager's retrofit of a new sound; see docs/Sound.md.
#include "SoundManager.h"
#include "SoundBuss.h"

void cSoundBussMan::RetroFitNewSound(SoundEntry *entry) {
    ApplyInactiveBusses(entry);
    dwi::list<cSoundBuss *>::iterator it(0);
    for (it = active.begin(); it != active.end(); it++) {
        if (*it)
            (*it)->RetroFit(entry);
    }
}
