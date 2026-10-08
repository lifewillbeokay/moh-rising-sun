#ifndef GAME_AI_SPLINE_PATH_H
#define GAME_AI_SPLINE_PATH_H
#include "BPD.h"
#include "AIMovement.h"
// AI-assisted reconstruction from GR8E69; see docs/Paths.md.
class CAISplinePathManager;
class CAISplinePathModule;
struct BSObject;
// Eighty-byte array stride, independently observed in generation/destruction.
// Coefficient names and parameterStep describe their observed use.
class CAISplinePathSegment {
public:
    unsigned char unknown00[4];
    float parameterStep;
    unsigned char unknown08[8];
    CVector3 cubic, quadratic, linear, constant;
    CAISplinePathSegment();
    ~CAISplinePathSegment();
    void Set(float, const CVector3 &, const CVector3 &, const CVector3 &, const CVector3 &);
    void Expand(float, CVector3 &);
    void ExpandDerivative(float, CVector3 &);
    void ExpandSecondDerivative(float, CVector3 &);
    float GetClosestParameter(float, const CVector3 &);
};
// The original array construction, destruction and lookup establish a 12-byte
// stride. Generation writes input point count minus one at +4; GetLastPoint
// indexes that entry and evaluates it at zero. Field names are descriptive.
class CAISplinePath {
public:
    CAISplinePathSegment *segments;
    unsigned int lastSegment;
    unsigned int id;
    CAISplinePath();
    ~CAISplinePath();
    void GetLastPoint(CVector3 *);
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
// Observed vector prefix only; no complete allocation size is established.
class CAISplinePathBreakdownPoint {
public:
    CVector3 position;
    CAISplinePathBreakdownPoint();
    ~CAISplinePathBreakdownPoint();
    bool WasCrossedDistance(const CVector3 &, float *);
};
// Observed prefix through the locomotion pointer at +0x20. Constructor and
// traversal accesses agree; this is not the complete navigation class.
class CAIAreaPathFinding {
public:
    unsigned char unknown00[0x20];
    CAILocomotion *locomotion;
    void StopAStarPathWalk();
    void UpdateAStarPathWalk();
    bool SetupAStarPathWalkForPatrol(const CVector3 &);
};
// Prefix through the next-point vector at +0x20. The untouched bytes at
// +0x18 and the complete allocation size remain unestablished.
class CAISplinePathTraversal {
public:
    int state;
    float parameter, previousDistanceSquared;
    unsigned int segmentIndex;
    CAISplinePath *path;
    CAISplinePathSegment *segment;
    unsigned char unknown18[8];
    CAISplinePathBreakdownPoint nextPoint;
    CAISplinePathTraversal();
    ~CAISplinePathTraversal();
    float GetCurrentParameterValue(float, const CVector3 &);
    void MakeModifiedNextPoint();
    void SetupNextSegmentForward(const CVector3 &);
    void PrepareForTraversalForward(CAISplinePath *, bool, CAIAreaPathFinding *);
    void RestartForwardTraversal(CAIAreaPathFinding *);
    void ContinueTraversalForwardAStar(CAIPhysics *, CAIAreaPathFinding *);
    void GetNextPointForward(const CVector3 &, const CVector3 &, CVector3 *, BSObject *, CAISplinePathModule *, CAIAreaPathFinding *);
    void GetFinalPointForward(CVector3 *);
    void GetCurrentDerivative(CVector3 *);
};
// Prefix through the traversal member at +0x20; not a complete class allocation
// or virtual interface. Original dispatch storage at zero is left opaque.
// These fragments reconstruct direct calls; construction/registration stay original.
class CAISplinePathModule {
public:
    unsigned char unknown00[4];
    CAIPhysics *physics;
    CAIAreaPathFinding *areaPathFinding;
    CAILocomotion *locomotion;
    BSObject *scriptObject;
    unsigned char unknown14[12];
    CAISplinePathTraversal traversal;
    void StartSplinePathWalkForward(CAISplinePath *);
    void WalkNextSplinePathPoint();
    void StopSplinePathWalk();
    void ContinueSplinePathWalk();
    void LoopSplinePathWalk();
    int GetWalkType();
    void StopWalk();
};
#endif
