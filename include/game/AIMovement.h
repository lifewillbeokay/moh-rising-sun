#ifndef GAME_AI_MOVEMENT_H
#define GAME_AI_MOVEMENT_H
#include "CVector3.h"
// AI-assisted reconstruction from GR8E69; see docs/Paths.md.
// Original tags; component names and composition describe observed storage.
// Sixteen bytes for the Euler vector and three consecutive 16-byte axis vectors.
struct CAIFilterRealEulerDirection { CVector3 angles; };
struct CAIFilterRealCoordinateAxes { CVector3 axis0, axis1, axis2; };
void CalculateDirectionsFromCoord(CAIFilterRealEulerDirection &, CAIFilterRealCoordinateAxes &);
// Observed prefix through +0x7f, not a complete allocation or inheritance model.
// The first sixteen bytes include original dispatch storage and remain opaque.
class CAIPhysics {
public:
    unsigned char unknown00[0x10];
    CVector3 position;
    CAIFilterRealEulerDirection eulerDirection;
    CAIFilterRealCoordinateAxes axes;
    float movementLength;
    unsigned char unknown64[12];
    CVector3 displacement;
    void UpdateOrientationFrom(const CVector3 &, const CVector3 &, const CVector3 &);
    void UpdatePositionFrom(const CVector3 &);
};
class CAITargeting;
class BSObject;
struct BS_STRUCT_Vector_struct;
// Original enum tags; enumerator names describe observed numeric selectors.
enum EMovePointType { MovePointType3 = 3, MovePointType4 = 4 };
enum AIOBJECT_PATH_WALK_TYPE {
    PathWalkTypeNone = 0,
    PathWalkTypeSpline = 1,
    PathWalkTypeAStar = 2,
    PathWalkTypeArbitrary = 3
};
// Dispatch-only interface: destructor, selector query, then stop. Both original
// spline/navigation tables corroborate this order. No subclass inheritance or
// complete interface allocation is established here.
class IAILocomotionMethod {
public:
    virtual ~IAILocomotionMethod();
    virtual int GetWalkType() = 0;
    virtual void StopWalk() = 0;
};
// Observed prefix through the teleport-request flag at +0x70, not a complete
// allocation. The original dispatch word and remaining gaps stay opaque.
class CAILocomotion {
public:
    unsigned char unknown00[4];
    CAIPhysics *physics;
    CAITargeting *targeting;
    BSObject *object;
    CVector3 objectivePosition;
    CVector3 arbitraryPosition;
    EMovePointType movePointType;
    float arrivalDistance;
    BS_STRUCT_Vector_struct *scriptPosition;
    int walkType;
    float updateTime;
    unsigned char unknown44[4];
    IAILocomotionMethod *methods[5];
    unsigned char unknown5c[4];
    CVector3 teleportPosition;
    int teleportPending;
    void StartWalkToArbitraryPoint(CVector3 &);
    void RegisterWalkType(IAILocomotionMethod *);
    void StopWalkByType(AIOBJECT_PATH_WALK_TYPE);
    float GetDistanceToArbitraryPoint();
    void SetupArbitraryPointUpdate(EMovePointType, BS_STRUCT_Vector_struct *, float);
    void UpdateArbitraryPoint();
    void WalkToArbitraryPoint();
    void CantReachTarget();
    void SetArbitraryWalkMPType(EMovePointType);
    void TeleportTo(const CVector3 &);
    CVector3 &GetCurrentObjectivePosition();
    void GetCurrentObjectivePosition(CVector3 &);
    void StopWalkToArbitraryPoint();
    void ContinueWalkToArbitraryPoint();
};
#endif
