// AI-assisted reconstruction; see docs/research/card-tasks.md.
#include "RealmcDriver.h"
namespace Realmc {
// The four callback slots are private symbols in memvectors.cpp.
extern void *(*gpMemAlloc)(const char *, int, int, int, int);
extern void (*gpMemFree)(void *);
extern void *(*gpMemAllocSize)(const char *, int, int, int, int);
extern void (*gpMemFreeSize)(void *, int);
void SetMemVectors(void *(*alloc)(const char *, int, int, int, int), void (*free)(void *)) {
    gpMemAlloc = alloc;
    gpMemFree = free;
}
void SetSizeMemVectors(void *(*alloc)(const char *, int, int, int, int), void (*free)(void *, int)) {
    gpMemAllocSize = alloc;
    gpMemFreeSize = free;
}
void *DefaultSizeAlloc(const char *name, int size, int a, int b, int c) {
    return gpMemAlloc(name, size, a, b, c);
}
void DefaultSizeFree(void *memory, int) { gpMemFree(memory); }
void *AllocateMem(const char *name, int size, int a, int b, int c) {
    return gpMemAlloc(name, size, a, b, c);
}
void *AllocateMemSize(const char *name, int size, int a, int b, int c) {
    return gpMemAllocSize(name, size, a, b, c);
}
void FreeMem(void *memory) { gpMemFree(memory); }
void FreeMemSize(void *memory, int size) { gpMemFreeSize(memory, size); }
}
