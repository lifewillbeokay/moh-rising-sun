// FEng's pooled allocation: nodes of fixed-size object arrays kept on a static list. FEPoolNode's
// name and arguments come from its table symbols, the file name from allocation labels. Member and
// method names are descriptive.
#ifndef GAME_FE_OBJECT_POOL_H
#define GAME_FE_OBJECT_POOL_H
#include "FEngLists.h"
extern "C" void *DWI_alloc(const char *, int, int);

template <class T, int N> class FEPoolNode : public FEMinNode {
  public:
    T objects[N];       // +0x0c
    FEMinList freeList;
    int used;
    FEPoolNode() : used(0) {
        for (int i = 0; i < N; i++) freeList.AddTail(&objects[i]);
    }
    void *operator new(unsigned int size, const char *label) { return DWI_alloc(label, size, 256); }
    virtual ~FEPoolNode();
    bool Contains(void *p) const { return p >= objects && p < objects + N; }
};
#endif
