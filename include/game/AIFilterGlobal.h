#ifndef GAME_AI_FILTER_GLOBAL_H
#define GAME_AI_FILTER_GLOBAL_H
#include "BPD.h"
class CAISplinePathManager;
class CAISplinePath;
// AI-assisted prefix through +0x13. The original singleton has additional
// storage; this is not an allocation type. See docs/Paths.md.
class CAIFilterGlobal {
public:
    unsigned int frameCount;
    float time; // Accumulated delta / 60; not a wall-clock timestamp.
    unsigned char unknown08[4];
    CAISplinePathManager *splineManager;
    unsigned int field10; // Constructor writes 0x2400; purpose not established.
    CAIFilterGlobal();
    void UpdateFromGlobals(float);
    void UpdateToGlobals();
    void CreateSplinePathManager(void *, BPDPolyPath *, int);
    void ResetSplinePathManager(void *, BPDPolyPath *, int);
    void ShutdownSplinePathManager();
    CAISplinePath *GetSplinePath(unsigned int, bool) const;
};
extern CAIFilterGlobal g_aigAIFilterGlobalObject;
#endif
