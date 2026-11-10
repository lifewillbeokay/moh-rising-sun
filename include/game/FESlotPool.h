// FEng's block allocator: pools of a fixed block size, each a list of nodes of 32 blocks tracked in a
// bit mask. Class tags and file name from symbols and allocation labels; member names are descriptive.
#ifndef GAME_FE_SLOT_POOL_H
#define GAME_FE_SLOT_POOL_H
#pragma interface
#include "FEngLists.h"
extern "C" void *DWI_alloc(const char *, int, int);
extern "C" void *memset(void *, int, unsigned int);

class FESlotNode : public FEMinNode {
  public:
    unsigned short blockSize; // +0x0c
    unsigned short used;      // +0x0e
    unsigned char mask[4];    // +0x10; bit n is set when block n is in use.
    unsigned char *blocks;    // +0x14
    FESlotNode(unsigned short size) : blockSize(size), used(0), blocks(0) {
        blocks = (unsigned char *)DWI_alloc("source/FEngine/FESlotPool.h:38", size * 32, 256);
        memset(mask, 0, 4);
    }
    virtual ~FESlotNode();
    void *operator new(unsigned int size, const char *label) { return DWI_alloc(label, size, 256); }
    unsigned char *AllocBlock();
    void FreeBlock(unsigned char *);
    bool Contains(unsigned char *p) const { return p >= blocks && p < blocks + blockSize * 32; }
};

class FESlotPool : public FEMinNode {
  public:
    FEMinList nodes;         // +0x0c
    unsigned long blockSize; // +0x1c
    FESlotPool(unsigned long size) : blockSize(size) {}
    virtual ~FESlotPool();
    void *operator new(unsigned int size, const char *label) { return DWI_alloc(label, size, 256); }
    unsigned char *Alloc();
    int Free(unsigned char *);
};

class FEMultiPool : public FEMinList {
  public:
    unsigned char *Alloc(unsigned long);
    void Free(unsigned char *);
};
#endif
