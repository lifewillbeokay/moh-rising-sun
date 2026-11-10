// A fragment of the original sys_memory.cpp: small-object pools in front of the general allocator, and
// the DWI_ allocation entry points. Function and global names come from the symbol table. The pool
// record's member names and the flag meanings are inferred from this code. The globals are the file's
// own; this fragment reaches them as externals.
#include <string.h>
#include <stdio.h>

extern "C" void *MEM_allocalign(const char *, int, int, int, int);
extern "C" int MEM_free(void *);
extern "C" int MEM_size(void *);
extern "C" void mallocinit();

struct SmallPool {
    char *memory;           // +0x00
    unsigned int *freeBits; // +0x04; one set bit per free element.
    int count;              // +0x08
    int freeWordCount;      // +0x0c
    int used;               // +0x10
    int elementWords;       // +0x14

    bool Contains(void *p) const {
        return (char *)p >= memory && (char *)p < memory + count * elementWords * 4;
    }
};

extern SmallPool g_smallPool[];
extern int g_numSmallPools;
extern int g_maxSmallSize;
extern const char *g_memLabel;
extern int g_bMemInitialized;
extern int g_classid;

// The file's private pool allocator and release; not reconstructed in this fragment.
void *allocSmall(const char *, int, int);
int freeSmall(void *);

static int sizeSmall(void *p) {
    for (int i = 0; i < g_numSmallPools; i++) {
        if (g_smallPool[i].Contains(p))
            return g_smallPool[i].elementWords * 4;
    }
    return MEM_size(p);
}

extern "C" int DWI_setclassid(int id) {
    int previous = g_classid;
    g_classid = id;
    return previous;
}

extern "C" void DWI_setallocid(const char *label) { g_memLabel = label; }

extern "C" const char *DWI_getallocid() { return g_memLabel; }

extern "C" void DWI_charfill(void *, int, int);

extern "C" void *DWI_alloc(const char *label, int size, int flags) {
    if (!g_bMemInitialized) {
        g_bMemInitialized = 1;
        mallocinit();
    }
    void *p = allocSmall(label, size, flags);
    int small = (flags >> 7) & 1;
    flags &= ~0x80;
    if (!p) {
        if (size > 256 && !small)
            flags |= 0x100;
        p = MEM_allocalign(label, size, 64, 0, g_classid | flags);
    }
    if (p)
        DWI_charfill(p, size, 0);
    else
        printf("\nMEMORY ALLOCATION ERROR: MEM_alloc failed for %d bytes. Out of memory?\n\n", size);
    return p;
}

extern "C" void *DWI_allocalign(const char *label, int size, int align, int flags) {
    if (align < 64)
        align = 64;
    int small = (flags >> 7) & 1;
    flags &= ~0x80;
    if (size > 256 && !small)
        flags |= 0x100;
    void *p = MEM_allocalign(label, size, align, 0, g_classid | flags);
    if (p)
        memset(p, 0, size);
    else
        printf("\nMEMORY ALLOCATION ERROR: MEM_allocalign failed for %d bytes. Out of memory?\n\n", size);
    return p;
}

extern "C" int DWI_free(void *p);
static inline const int &Smaller(const int &a, const int &b) { return b < a ? b : a; }

extern "C" void *DWI_realloc(const char *label, void *p, int size, int flags) {
    if (!p)
        return DWI_alloc(label, size, flags);
    int oldSize = sizeSmall(p);
    if (size == oldSize)
        return p;
    void *q = DWI_alloc(label, size, flags);
    if (q)
        memcpy(q, p, Smaller(oldSize, size));
    DWI_free(p);
    return q;
}

extern "C" int DWI_free(void *p) {
    if (freeSmall(p))
        return 1;
    return MEM_free(p);
}

extern "C" void DWI_charfill(void *p, int size, int value) {
    for (int i = 0; i < size; i++)
        ((char *)p)[i] = value;
}

extern "C" void DWI_intfill(void *p, int size, int value) {
    int *word = (int *)p;
    for (unsigned int i = (unsigned int)size / 4; i; i--)
        *word++ = value;
}
