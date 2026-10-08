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
// Observed prefix through walkType at +0x3c. Do not allocate from this view.
// The dispatch word and other unknown fields retain their original ownership.
class CAILocomotion {
public:
    unsigned char unknown00[0x10];
    CVector3 objectivePosition;
    unsigned char unknown20[0x1c];
    int walkType;
    CVector3 &GetCurrentObjectivePosition();
    void GetCurrentObjectivePosition(CVector3 &);
    void StopWalkToArbitraryPoint();
    void ContinueWalkToArbitraryPoint();
};
#endif
