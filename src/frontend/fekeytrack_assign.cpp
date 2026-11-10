// AI-assisted reconstruction from GR8E69; see docs/research/feng-animation-lists.md.
#ifdef DECOMP_COMPARE
#include "FEScript.h"

void FEKeyTrack::operator=(FEKeyTrack &source) {
    FEMinNode *old;
    while ((old = keys.RemHead())) delete old;
    unknown_0 = source.unknown_0;
    valueSize = source.valueSize;
    unknown_2 = source.unknown_2;
    flags = source.flags;
    lastKeyTime = source.lastKeyTime;
    dataOffset = source.dataOffset;
    baseKey.time = source.baseKey.time;
    baseKey.value = source.baseKey.value;
    if (source.keys.isReference) {
        keys.ReferenceList((FERefList *)source.keys.head);
    } else {
        for (FEKeyNode *node = (FEKeyNode *)source.keys.head; node; node = (FEKeyNode *)node->next) {
            FEKeyNode *copy = new FEKeyNode;
            copy->time = node->time;
            copy->value = node->value;
            keys.AddNode(keys.tail, copy);
        }
    }
}

#endif
