// AI-assisted reconstruction from GR8E69; see docs/Sound.md.
#include "SoundManager.h"
#include "SoundEvents.h"
SoundEntry::~SoundEntry() {
    if (emitter) {
        emitter->~SoundEmitter();
        emitter = 0;
    }
}
