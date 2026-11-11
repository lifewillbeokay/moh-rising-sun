// AI-assisted GR8E69 reconstruction; see docs/research/feng-packages.md.
#ifdef DECOMP_COMPARE
#include "FEngInterpolation.h"
#include "FEngQuaternionOps.h"
void FEObject::SetupMoveToTracks() {
    FEKeyTrack *track = currentScript->tracks;
    unsigned long count = currentScript->trackCount;
    for (unsigned long i = 0; i < count; ++i, ++track) {
        track->flags &= 0x7f;
        if ((unsigned int)(track->unknown_2 - 3) >= 2) continue;
        float *value = (float *)data + track->dataOffset;
        FEKeyNode *key = (FEKeyNode *)track->keys.GetHead();
        if (!key) continue;
        float *base = (float *)&track->baseKey.value, *out = (float *)&key->value;
        switch (track->unknown_0) {
        case 1: *(long *)out = *(long *)value - *(long *)base; break;
        case 2: *out = *value - *base; break;
        case 3: out[0] = value[0] - base[0]; out[1] = value[1] - base[1]; break;
        case 4:
            out[0] = value[0] - base[0]; out[1] = value[1] - base[1]; out[2] = value[2] - base[2]; break;
        case 5: {
            FEQuaternion inverse = {-base[0], -base[1], -base[2], base[3]};
            *(FEQuaternion *)out = FEQuatMultiply(*(FEQuaternion *)value, inverse); break;
        }
        case 6: *(FEColor *)out = *(FEColor *)value - *(FEColor *)base; break;
        }
    }
}
#endif
