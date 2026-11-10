// AI-assisted reconstruction of EALA::Character's pool allocator; see docs/Animation.md.
// Member names are inferred; the free list's links sit in the free blocks.
#ifndef MOH_CHARACTERPOOL_H
#define MOH_CHARACTERPOOL_H
#pragma interface
namespace EALA {
namespace Character {
class PoolAllocator {
  public:
    PoolAllocator(const char *name, unsigned int size);
    virtual ~PoolAllocator();
    void *Alloc(unsigned int size);
    void Free(void *p);
    void Dump();
    struct Block {
        unsigned int size;
        Block *previous;
        Block *next;
    };
    static PoolAllocator *m_pHead;

  private:
    const char *name;          // +0x00
    unsigned int size;         // +0x04
    Block *base;               // +0x08
    Block *freeList;           // +0x0c
    PoolAllocator *previous;   // +0x10
    PoolAllocator *next;       // +0x14
    unsigned int used;         // +0x18
    unsigned int peak;         // +0x1c
};
} // namespace Character
} // namespace EALA
#endif
