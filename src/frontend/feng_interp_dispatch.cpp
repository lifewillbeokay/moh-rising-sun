// AI-assisted reconstruction from GR8E69; see docs/research/feng-animation-lists.md.
#ifdef DECOMP_COMPARE
#include "FEngInterpolation.h"

void FEKeyInterp(FEScript *script, unsigned char index, long time, FEObject *object) {
    switch (script->tracks[index].unknown_2) {
    case 0: FEInterpNone(script, index, time, object->data); break;
    case 1:
    case 3: FEInterpLinear(script, index, time, object->data); break;
    case 2: break;
    }
}

void FEKeyInterp(FEKeyTrack *track, long time, void *out) {
    switch (track->unknown_2) {
    case 0: FEInterpNone(track, time, out); break;
    case 1:
    case 3: FEInterpLinear(track, time, out); break;
    case 2: break;
    }
}

void FEKeyInterpFast(FEKeyTrack *track, long time, void *out) {
    if (!(track->flags & 0x80)) {
        switch (track->unknown_2) {
        case 0: FEInterpNone(track, time, out); break;
        case 1:
        case 3: FEInterpLinear(track, time, out); break;
        case 2: break;
        }
        if (!track->keys.GetNumElements()) track->flags |= 0x80;
    }
}

#endif
