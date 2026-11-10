// AI-assisted reconstruction from GR8E69; see docs/research/feng-animation-lists.md.
#ifdef DECOMP_COMPARE
#include "FEScript.h"

FEKeyNode *FEKeyTrack::GetKeyAt(long time) {
    FEKeyNode *key;
    if (time < 0 || !(key = (FEKeyNode *)keys.GetHead())) return &baseKey;
    while (key->next && key->time < time) key = (FEKeyNode *)key->next;
    return key;
}

FEKeyNode *FEKeyTrack::GetDeltaKeyAt(long time) {
    FEKeyNode *key = (FEKeyNode *)keys.GetHead();
    if (!key) return 0;
    while (key->next && key->time < time) key = (FEKeyNode *)key->next;
    return key;
}

void FEKeyTrack::InsertKey(FEKeyNode *key) {
    FEKeyNode *after = (FEKeyNode *)keys.GetHead();
    if (after) {
        if (key->time < after->time) {
            keys.AddNode(0, key);
            return;
        }
        while (after) {
            if (!after->next || key->time < ((FEKeyNode *)after->next)->time) break;
            after = (FEKeyNode *)after->next;
        }
    }
    keys.AddNode(after, key);
}

#endif
