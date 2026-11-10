// AI-assisted reconstruction from GR8E69; see docs/research/feng-animation-lists.md.
#ifdef DECOMP_COMPARE
#include "FEngInterpolation.h"
#include <string.h>

void FEInterpNone(FEScript *script, unsigned char index, long time, void *out) {
    FEKeyTrack *track = &script->tracks[index];
    FEInterpNone(track, time, (long *)out + track->dataOffset);
}

void FEInterpNone(FEKeyTrack *track, long time, void *out) {
    unsigned long size = track->valueSize;
    FEKeyNode *key;
    if (time <= track->lastKeyTime) {
        key = track->GetKeyAt(time);
        if (key->prev && key->time > time) key = (FEKeyNode *)key->prev;
    } else {
        key = track->GetKeyAt(track->lastKeyTime - time);
        if (key->next && key->time < time) key = (FEKeyNode *)key->next;
    }
    memcpy(out, &key->value, size);
}

#endif
