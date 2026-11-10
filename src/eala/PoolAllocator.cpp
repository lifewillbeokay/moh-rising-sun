// AI-assisted reconstruction of source/Character/PoolAllocator.cpp; see docs/Animation.md.
#include "profile.h"
#include "CharacterPool.h"
extern "C" void *MEM_alloc(const char *name, unsigned int size, int flags);
extern "C" int MEM_free(void *);
extern "C" void *DWI_alloc(const char *, unsigned int, unsigned int);
extern "C" void DWI_free(void *);
void DebugMsg(const char *, ...);
namespace EALA {
namespace Character {

PoolAllocator::PoolAllocator(const char *name, unsigned int size) : name(name), size(size) {
    used = 0;
    peak = 0;
    int excess = size - size / sizeof(Block) * sizeof(Block);
    if (excess) {
        unsigned int rounded = size + sizeof(Block);
        rounded -= excess;
        this->size = rounded;
    }
    base = freeList = (Block *)MEM_alloc(name, this->size, 0);
    base->previous = base->next = 0;
    freeList->size = this->size;
    previous = 0;
    next = m_pHead;
    if (next)
        next->previous = this;
    m_pHead = this;
}

PoolAllocator::~PoolAllocator() {
    MEM_free(base);
    if (previous)
        previous->next = next;
    if (next)
        next->previous = previous;
    if (m_pHead == this)
        m_pHead = next;
}

void *PoolAllocator::Alloc(unsigned int size) {
    unsigned int total = size + 4;
    if (total & 3)
        total += 4 - (total & 3);
    Block *block = freeList;
    while (block && block->size < total)
        block = block->next;
    if (block) {
        unsigned int remaining = block->size - total;
        if (remaining > 11) {
            Block *rest = (Block *)((char *)block + total);
            rest->size = remaining;
            rest->previous = block->previous;
            rest->next = block->next;
            if (block->previous)
                block->previous->next = rest;
            if (block->next)
                block->next->previous = rest;
            if (freeList == block)
                freeList = rest;
        } else {
            if (block->previous)
                block->previous->next = block->next;
            if (block->next)
                block->next->previous = block->previous;
            if (freeList == block)
                freeList = block->next;
        }
        block->size = total;
        void *result = (char *)block + 4;
        used += total;
        if (peak < used)
            peak = used;
        return result;
    }
    DebugMsg("\n!!!! ALLOCATION FAILURE !!!!\n\n");
    DebugMsg("The animation system failed to allocate from a pool.\n");
    DebugMsg("Please contact your friendly neighborhood engineer\n");
    DebugMsg("and have them increase the proper memory pool.\n\n");
    DebugMsg("Pool Information:\n");
    Dump();
    return DWI_alloc(name, total, 0);
}

void PoolAllocator::Free(void *p) {
    char *start = (char *)base;
    char *end = start + size;
    if (p < start || p >= end) {
        DWI_free(p);
        return;
    }
    Block *block = (Block *)((char *)p - 4);
    block->previous = 0;
    block->next = freeList;
    if (freeList)
        freeList->previous = block;
    freeList = block;
    used -= block->size;
}

void PoolAllocator::Dump() {
    DebugMsg("\t%23s:  U: %5d (%5d) / %5d.  F: %5d (%5d).\n", name, used, peak, size, size - used, size - peak);
}

} // namespace Character
} // namespace EALA
