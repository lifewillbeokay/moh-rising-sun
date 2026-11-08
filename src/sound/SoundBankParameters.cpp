// AI-assisted reconstruction from GR8E69; see docs/Sound.md.
#include "SoundManager.h"
extern "C" int SNDpitchmult(int, int);
extern "C" int SNDvol(int, int);
// Descriptive clamp helper, not a recovered original name.
static inline int ClipInteger(int x) {
    if (!(x > 127)) {
        if (x >= 0) return x;
        else return 0;
    }
    return 127;
}
void SoundEntryBank::SetSndPitch(int pitch) { SNDpitchmult(handle, pitch); }
void SoundEntryBank::SetNormSndVol(float volume) {
    int level = (int)((volume * g_SoundMan.soundVolume) * 127.0f + 0.5f);
    int clipped = ClipInteger(level);
    SNDvol(handle, clipped);
}
