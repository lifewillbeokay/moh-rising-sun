// AI-assisted reconstruction from GR8E69; see docs/Sound.md.
#include "SoundManager.h"
#include "SoundEvents.h"
#include "SoundPlayOptions.h"
extern "C" int SNDBANK_play(int, int, SNDPLAYOPTS *);
extern "C" void SNDSYS_entercritical();
extern "C" void SNDSYS_leavecritical();
struct SNDFILTERDEF;
extern "C" int SNDCTRL_filteradd(int, SNDFILTERDEF *);
// The byte conversion is intentionally after the floating comparisons.
static inline unsigned char ClipVolume(float level) {
    if (!(level > 127.0f)) {
        if (!(level < 0.0f)) return level;
        return 0;
    }
    return 127;
}
int SoundEntryBank::Play() {
    SNDPLAYOPTS opts;
    GetOpts(opts, currentVolume);
    opts.byte0 = ClipVolume((currentVolume * g_SoundMan.soundVolume) * 127.0f + 0.5f);
    int sound = SNDBANK_play(field_d8, patch, &opts);
    if (sound >= 0) {
        handle = sound;
        if (filterEnabled && emitter) {
            void *filter = emitter->GetFilterDef();
            if (filter) {
                SNDSYS_entercritical();
                SNDCTRL_filteradd(handle, (SNDFILTERDEF *)filter);
                SNDSYS_leavecritical();
            }
        }
        return 0;
    }
    handle = -12;
    return -1;
}
