#ifndef GAME_SCRIPT_POOL_H
#define GAME_SCRIPT_POOL_H
// GR8E69 allocator storage. Field names and PayloadSize are descriptive.
// The original object symbols and array stride independently establish 28 bytes.
class BSUtilObjectInstanceMemoryAllocator {
public:
    int payloadWords;
    int strideWords;
    int elementCount;
    int allocatedCount;
    int totalWords;
    int *base;
    int *freeHead;
    int PayloadSize() { return payloadWords * 4; }
    int ComputeElementSize(int, int);
    int ComputeNeededSize(int, int, int);
    void Init(void *, int, int, int);
    void *GetFreeElement(bool);
    void FreeElement(void *);
    void MakeHeap();
    int DoWeOwnThisMemory(void *);
    int GetIndexOfObject(void *);
    void *GetObjectByIndex(int);
};
typedef char BSScriptPoolStorageCheck[sizeof(BSUtilObjectInstanceMemoryAllocator) == 28 ? 1 : -1];
#endif
