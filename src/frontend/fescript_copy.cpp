// AI-assisted GR8E69 reconstruction; see docs/research/feng-packages.md.
#ifdef DECOMP_COMPARE
#include "FEScript.h"
FEScript::FEScript(FEScript &other, bool reference) {
    Init(); SetName(other.name);
    unknown_c = other.unknown_c; time = other.time; unknown_14 = other.unknown_14;
    SetTrackCount(other.trackCount);
    if (reference) {
        for (unsigned long i = 0; i < (unsigned long)trackCount; ++i) {
            FEKeyTrack &out = tracks[i], &in = other.tracks[i];
            out.unknown_0 = in.unknown_0; out.valueSize = in.valueSize;
            out.unknown_2 = in.unknown_2; out.flags = in.flags;
            out.lastKeyTime = in.lastKeyTime; out.dataOffset = in.dataOffset;
            out.baseKey = in.baseKey;
            out.keys.ReferenceList(&in.keys);
        }
    } else {
        for (unsigned long i = 0; i < (unsigned long)trackCount; ++i) tracks[i] = other.tracks[i];
    }
    events = other.events;
}
#endif
