// AI-assisted GR8E69 reconstruction; see docs/research/feng-packages.md.
#ifdef DECOMP_COMPARE
#include "FEngInterpolation.h"
void FEPackage::UpdateObjectTracks(FEObject *object, FEScript *script) {
    long time = script->time;
    unsigned long *values = (unsigned long *)object->data;
    FEKeyTrack *track = script->tracks;
    if (!executing) {
        if (track && !track->dataOffset) FEKeyInterp(track, time, values);
        if (((FEObjData *)object->data)->visible) {
            unsigned char count = script->trackCount;
            for (unsigned char i = 0; i < count; ++i, ++track)
                FEKeyInterp(track, time, values + track->dataOffset);
        }
    } else {
        if (track && !track->dataOffset) {
            if (!(track->flags & 0x80)) object->flags |= 0x800000;
            else object->flags &= uHoldDirtyFlags | 0xff7fffff;
            FEKeyInterpFast(track, time, values);
        } else object->flags &= uHoldDirtyFlags | 0xff7fffff;
        unsigned char cached = 0x80;
        if (((FEObjData *)object->data)->visible) {
            unsigned char count = script->trackCount;
            for (unsigned char i = 0; i < count; ++i, ++track) {
                if (track->dataOffset < 16) cached &= track->flags;
                FEKeyInterpFast(track, time, values + track->dataOffset);
            }
        }
        if (!cached) object->flags |= 0x1000000;
        else object->flags &= uHoldDirtyFlags | 0xfeffffff;
    }
    if (object->flags & 0x1c00000) object->flags |= 0x2000000;
}
#endif
