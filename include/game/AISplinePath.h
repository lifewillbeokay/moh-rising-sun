#ifndef GAME_AI_SPLINE_PATH_H
#define GAME_AI_SPLINE_PATH_H
#include "BPD.h"
// AI-assisted reconstruction from GR8E69; see docs/Paths.md.
class CAISplinePathManager;
class CAISplinePathSegment;
// The original array construction, destruction and lookup establish a 12-byte
// stride. Field names are descriptive; the word at +4 remains opaque.
class CAISplinePath {
public:
    CAISplinePathSegment *segments;
    unsigned char unknown04[4];
    unsigned int id;
    CAISplinePath();
    ~CAISplinePath();
    void GenerateTestSplinePath(const CAISplinePathManager &, unsigned int, const PropVec3 *, bool);
};
// Its allocation and six constructor stores establish 24 bytes. Buffer element
// types remain unspecified here; original spline math consumes them.
class CAISplinePathManager {
public:
    unsigned int pathCount;
    CAISplinePath *paths;
    void *buffer08, *buffer0c, *buffer10, *buffer14;
    CAISplinePathManager(unsigned int);
    ~CAISplinePathManager();
    void AllocateGenerateBuffers();
    void FreeGenerateBuffers();
    void GenerateSplinePath(unsigned int, unsigned int, unsigned int, const PropVec3 *);
};
#endif
