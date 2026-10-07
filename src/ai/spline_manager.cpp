#include "AISplinePath.h"
extern "C" void *DWI_alloc(const char *, int, int);
extern "C" int DWI_free(void *);
inline void *operator new[](unsigned int n, const char *label) { return DWI_alloc(label, n, 1024); }

CAISplinePathManager::CAISplinePathManager(unsigned int count) {
    pathCount = count * 2;
    paths = new ("source/ai_core/AI_side/AISplinePath.cpp:1187") CAISplinePath[pathCount];
    buffer08 = 0;
    buffer0c = 0;
    buffer10 = 0;
    buffer14 = 0;
}
CAISplinePathManager::~CAISplinePathManager() {
    delete[] paths;
    paths = 0;
}
void CAISplinePathManager::AllocateGenerateBuffers() {
    buffer08 = DWI_alloc(0, 16384, 256);
    buffer0c = DWI_alloc(0, 4096, 256);
    buffer10 = DWI_alloc(0, 16384, 256);
    buffer14 = DWI_alloc(0, 16384, 256);
}
void CAISplinePathManager::FreeGenerateBuffers() {
    DWI_free(buffer08);
    DWI_free(buffer0c);
    DWI_free(buffer10);
    DWI_free(buffer14);
    buffer08 = 0;
    buffer0c = 0;
    buffer10 = 0;
    buffer14 = 0;
}
void CAISplinePathManager::GenerateSplinePath(unsigned int index, unsigned int id, unsigned int count, const PropVec3 *points) {
    paths[index].GenerateTestSplinePath(*this, count, points, false);
    paths[index].id = id;
    index += pathCount / 2;
    paths[index].GenerateTestSplinePath(*this, count, points, true);
    paths[index].id = id + 0x1000000;
}
