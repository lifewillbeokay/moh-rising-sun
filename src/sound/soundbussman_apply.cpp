// AI-assisted reconstruction of the sound buss manager's active buss application; see docs/Sound.md.
#include "SoundManager.h"
#include "SoundBuss.h"

void cSoundBussMan::ApplyActiveBusses(SoundEntry *entry) {
    dwi::list<cSoundBuss *>::iterator it(0);
    for (it = active.begin(); it != active.end(); it++) {
        if (*it)
            (*it)->UpdateBussEntry(entry);
    }
}
