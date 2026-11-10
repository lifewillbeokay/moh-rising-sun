// FEKeyTrack's keys and their pooled allocation.
#include "FEScript.h"
typedef FEPoolNode<FEKeyNode, 256> FEKeyNodePoolNode;
class FEColor {
  public:
    FEColor &operator=(const FEColor &);
    FEColor operator-(const FEColor &) const;
    int b, g, r, a;
};

void *FEKeyNode::operator new(unsigned int) {
    FEMinList &pool = NodePool;
    FEKeyNodePoolNode *node;
    for (node = (FEKeyNodePoolNode *)pool.head; node; node = (FEKeyNodePoolNode *)node->next) {
        if (node->freeList.count) break;
    }
    if (!node) {
        node = new ("source/FEngine/ObjectPool.h:87") FEKeyNodePoolNode;
        pool.AddNode(0, node);
    }
    FEKeyNode *object = (FEKeyNode *)node->freeList.RemHead();
    node->used++;
    return object;
}
void FEKeyNode::operator delete(void *p) {
    ((FEKeyNode *)p)->~FEKeyNode();
    FEMinList &pool = NodePool;
    for (FEKeyNodePoolNode *node = (FEKeyNodePoolNode *)pool.head; node; node = (FEKeyNodePoolNode *)node->next) {
        if (node->Contains(p)) {
            node->freeList.AddTail((FEKeyNode *)p);
            if (--node->used == 0) {
                pool.RemNode(node);
                delete node;
            }
            return;
        }
    }
}
