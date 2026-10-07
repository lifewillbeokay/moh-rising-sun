#include "BPD.h"
#include "AIFilterGlobal.h"
#include "AISplinePath.h"
#include "Endian.h"
extern "C" void *DWI_alloc(const char *, int, int);
inline void *operator new(unsigned int, void *p) { return p; }
// Original static reset interfaces; no object layout is needed.
class CAreaSearchNode { public: static void Reset(); };
class CAIObject { public: static void ResetGlobalList(); };

void CAIFilterGlobal::CreateSplinePathManager(void *base, BPDPolyPath *paths, int count) {
    if (count) {
        splineManager = new (DWI_alloc("source/ai_core/Filter/AIFilter.cpp:391", sizeof(CAISplinePathManager), 1024)) CAISplinePathManager(count);
        splineManager->AllocateGenerateBuffers();
        for (int i = 0; i < count; ++i, ++paths) {
            ChangeEndian(paths->id);
            ChangeEndian(paths->pointCount);
            ChangeEndian(paths->points);
            paths->points = (PropVec3 *)((int)base + (int)paths->points);
            for (int j = 0; j < paths->pointCount; ++j) {
                PropVec3 *point = &paths->points[j];
                ChangeEndian(point->x);
                ChangeEndian(point->y);
                ChangeEndian(point->z);
            }
            splineManager->GenerateSplinePath(i, paths->id, paths->pointCount, paths->points);
        }
        splineManager->FreeGenerateBuffers();
    }
}
void CAIFilterGlobal::ResetSplinePathManager(void *, BPDPolyPath *, int) {
    CAreaSearchNode::Reset();
    CAIObject::ResetGlobalList();
}
void CAIFilterGlobal::ShutdownSplinePathManager() {
    delete splineManager;
    splineManager = 0;
}
CAISplinePath *CAIFilterGlobal::GetSplinePath(unsigned int id, bool reverse) const {
    if (reverse)
        id += 0x1000000;
    CAISplinePathManager *manager = splineManager;
    unsigned int i;
    for (i = 0; i < manager->pathCount && manager->paths[i].id != id; ++i) {}
    // Preserve the original one-past-end result when the ID is absent.
    return &manager->paths[i];
}
