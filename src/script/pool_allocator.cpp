#include "ScriptPool.h"
int BSUtilObjectInstanceMemoryAllocator::ComputeElementSize(int size, int alignment) {
    size += 4;
    if (alignment) {
        int remainder = size & (alignment - 1);
        if (remainder) size += alignment - remainder;
    }
    return size;
}
int BSUtilObjectInstanceMemoryAllocator::ComputeNeededSize(int size, int count, int alignment) {
    int total = ComputeElementSize(size, alignment) * count;
    return total + (16 - (total & 15));
}
void BSUtilObjectInstanceMemoryAllocator::Init(void *memory, int size, int count, int alignment) {
    base = (int *)memory;
    elementCount = count;
    payloadWords = size >> 2;
    strideWords = ComputeElementSize(size, alignment) >> 2;
    allocatedCount = 0;
    totalWords = ComputeNeededSize(size, count, alignment) >> 2;
    MakeHeap();
}
void *BSUtilObjectInstanceMemoryAllocator::GetFreeElement(bool) {
    if (allocatedCount < elementCount) {
        int *result = freeHead;
        freeHead = (int *)result[payloadWords];
        ++allocatedCount;
        return result;
    }
    return 0;
}
void BSUtilObjectInstanceMemoryAllocator::FreeElement(void *memory) {
    ((int *)memory)[payloadWords] = (int)freeHead;
    freeHead = (int *)memory;
    --allocatedCount;
}
void BSUtilObjectInstanceMemoryAllocator::MakeHeap() {
    int *link = base + payloadWords;
    int step = strideWords - payloadWords;
    for (int i = 0; i < elementCount - 1; ++i) {
        *link = (int)(link + step);
        link += strideWords;
    }
    *link = 0;
    freeHead = base;
}
int BSUtilObjectInstanceMemoryAllocator::DoWeOwnThisMemory(void *memory) {
    int offset = (char *)memory - (char *)base;
    return memory >= base && memory < base + totalWords && offset % (strideWords * 4) == 0;
}
int BSUtilObjectInstanceMemoryAllocator::GetIndexOfObject(void *memory) {
    return ((char *)memory - (char *)base) / strideWords;
}
void *BSUtilObjectInstanceMemoryAllocator::GetObjectByIndex(int index) {
    return (char *)base + index * strideWords;
}
