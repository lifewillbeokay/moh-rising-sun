// AI-assisted reconstruction from GR8E69; see docs/research/feng-animation-lists.md.
#include "FEngInterpolation.h"

void FEInterpLinear(FEScript *script, unsigned char index, long time, void *out) {
    FEKeyTrack *track = &script->tracks[index];
    FEInterpLinear(track, time, (long *)out + track->dataOffset);
}
