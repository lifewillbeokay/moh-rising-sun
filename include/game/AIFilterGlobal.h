#ifndef GAME_AI_FILTER_GLOBAL_H
#define GAME_AI_FILTER_GLOBAL_H
#include "BPD.h"
class CAISplinePathManager;
class CAISplinePath;
// Scoped view: the spline manager is at +0x0c. The original singleton has
// additional storage; this prefix is not an allocation type. See docs/Paths.md.
class CAIFilterGlobal {
public:
    unsigned char unknown00[12];
    CAISplinePathManager *splineManager;
    void CreateSplinePathManager(void *, BPDPolyPath *, int);
    void ResetSplinePathManager(void *, BPDPolyPath *, int);
    void ShutdownSplinePathManager();
    CAISplinePath *GetSplinePath(unsigned int, bool) const;
};
extern CAIFilterGlobal g_aigAIFilterGlobalObject;
#endif
