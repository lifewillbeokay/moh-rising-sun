// AI-assisted scoped reconstruction from GR8E69.
#ifndef GAME_MEM_STACK_H
#define GAME_MEM_STACK_H
extern "C" void *DWI_alloc(const char *, int, int);
// Twelve-byte target storage; member names are descriptive. See docs/Memory.md.
class MemStack {
public:
    unsigned char *base;
    unsigned char *cursor;
    int capacity;
    MemStack(int);
    ~MemStack();
    void *Alloc(int, int); // Original method remains external.
    static void *operator new(unsigned int size, const char *label) {
        return DWI_alloc(label, size, 1024);
    }
};
#endif
