// AI-assisted reconstruction from GR8E69; see docs/Sound.md.
#include "SoundManager.h"
#include "SoundPlayOptions.h"
// Descriptive helper; the unordered lower-bound path produces zero.
static inline unsigned char ConvertVolume(float v) {
    float level = v * 127.0f + 0.5f;
    if (!(level > 127.0f)) {
        if (!(level >= 0.0f)) level = 0.0f;
        return level;
    }
    return 127;
}
void SoundEntry::GetOpts(SNDPLAYOPTS &opts, float &v) {
    SNDplaysetdef(&opts);
    if (field_04 == 1) {
        v = CalculateVolDistance();
        opts.short8 = azimuth;
    } else v = volume;
    opts.byte0 = ConvertVolume(v);
    opts.shortC = pitch;
}
