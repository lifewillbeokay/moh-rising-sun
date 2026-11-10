// FEScript.cpp: scripts, their tracks and their pooled allocation.
#include "FEScript.h"
#include "FEHash.h"
#include <string.h>
extern const long FETrackOffsets[11];
const long FETrackOffsets[11] = {0, 4, 7, 10, 14, 17, 19, 21, 25, 29, 33};
typedef FEPoolNode<FEScript, 32> FEScriptPoolNode;
inline void *operator new[](unsigned int size, const char *label) { return DWI_alloc(label, size, 256); }

void *FEScript::operator new(unsigned int) {
    FEMinList &pool = NodePool;
    FEScriptPoolNode *node;
    for (node = (FEScriptPoolNode *)pool.head; node; node = (FEScriptPoolNode *)node->next) {
        if (node->freeList.count) break;
    }
    if (!node) {
        node = new ("source/FEngine/ObjectPool.h:87") FEScriptPoolNode;
        pool.AddNode(0, node);
    }
    FEScript *object = (FEScript *)node->freeList.RemHead();
    node->used++;
    object->Init();
    return object;
}
void FEScript::operator delete(void *p) {
    ((FEScript *)p)->~FEScript();
    FEMinList &pool = NodePool;
    for (FEScriptPoolNode *node = (FEScriptPoolNode *)pool.head; node; node = (FEScriptPoolNode *)node->next) {
        if (node->Contains(p)) {
            node->freeList.AddTail((FEScript *)p);
            if (--node->used == 0) {
                pool.RemNode(node);
                delete node;
            }
            return;
        }
    }
}
void FEScript::SetName(const char *text) {
    if (name) {
        delete[] name;
        name = 0;
    }
    hash = (unsigned long)-1;
    if (text) {
        name = new ("source/FEngine/FEScript.cpp:145") char[strlen(text) + 1];
        strcpy(name, text);
        hash = FEHashUpper(name);
    }
}
void FEScript::SetTrackCount(long count) {
    if (tracks) delete[] tracks;
    trackCount = count;
    tracks = 0;
    if (count) tracks = new ("source/FEngine/FEScript.cpp:159") FEKeyTrack[count];
}
FEKeyTrack *FEScript::FindTrack(FEKeyTrack_Indices index) const {
    long offset = FETrackOffsets[index];
    for (unsigned long i = 0; i < trackCount; i++) {
        if (tracks[i].dataOffset == offset) return &tracks[i];
    }
    return 0;
}
