// FESlotPool.cpp: the block allocator behind FEng's object data.
#include "FESlotPool.h"

unsigned char *FESlotNode::AllocBlock() {
    if (used == 32) return 0;
    unsigned int index = 0;
    while (mask[index] == 255) index++;
    index *= 8;
    while ((mask[index >> 3] >> (index & 7)) & 1) index++;
    used++;
    mask[index >> 3] |= 1 << (index & 7);
    return blocks + blockSize * index;
}
void FESlotNode::FreeBlock(unsigned char *p) {
    unsigned int index = (unsigned int)(p - blocks) / blockSize;
    used--;
    mask[index >> 3] &= ~(1 << (index & 7));
}
unsigned char *FESlotPool::Alloc() {
    FESlotNode *node;
    for (node = (FESlotNode *)nodes.head; node; node = (FESlotNode *)node->next) {
        if (node->used != 32) break;
    }
    if (!node) {
        node = new ("source/FEngine/FESlotPool.cpp:52") FESlotNode(blockSize);
        nodes.AddNode(0, node);
    }
    return node->AllocBlock();
}
int FESlotPool::Free(unsigned char *p) {
    FESlotNode *node;
    for (node = (FESlotNode *)nodes.head; node; node = (FESlotNode *)node->next) {
        if (node->Contains(p)) break;
    }
    if (!node) return 0;
    node->FreeBlock(p);
    if (node->used == 0) {
        nodes.RemNode(node);
        delete node;
    }
    return 1;
}
unsigned char *FEMultiPool::Alloc(unsigned long size) {
    if (!size) return 0;
    FESlotPool *pool;
    for (pool = (FESlotPool *)head; pool; pool = (FESlotPool *)pool->next) {
        if (pool->blockSize == size) break;
    }
    if (!pool) {
        pool = new ("source/FEngine/FESlotPool.cpp:93") FESlotPool(size);
        AddNode(0, pool);
    }
    return pool->Alloc();
}
void FEMultiPool::Free(unsigned char *p) {
    if (p) {
        FESlotPool *pool;
        for (pool = (FESlotPool *)head; pool; pool = (FESlotPool *)pool->next) {
            if (pool->Free(p)) break;
        }
        if (pool->nodes.count == 0) {
            RemNode(pool);
            delete pool;
        }
    }
}
