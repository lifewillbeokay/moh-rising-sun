#ifndef GAME_FENG_INTERPOLATION_H
#define GAME_FENG_INTERPOLATION_H

// FEng keyframe operations named by the GR8E69 symbols. Argument roles and
// the two-float FEVector2 view are inferred from their loads and stores.
#include "FEngMath.h"
#include "FEColor.h"

struct FEVector2 {
    float x, y;
    FEVector2 &operator=(const FEVector2 &);
};

void FEKeyInterp(FEScript *, unsigned char, long, FEObject *);
void FEKeyInterp(FEKeyTrack *, long, void *);
void FEKeyInterpFast(FEKeyTrack *, long, void *);
void FEInterpNone(FEScript *, unsigned char, long, void *);
void FEInterpNone(FEKeyTrack *, long, void *);
void FEInterpLinear(FEScript *, unsigned char, long, void *);
void FEInterpLinear(FEKeyTrack *, long, void *);
void FELerpInteger(long, long, float, long *, long *);
void FELerpFloat(float, float, float, float *, float *);
void FELerpVector2(FEVector2 &, FEVector2 &, float, FEVector2 *, FEVector2 *);
void FELerpVector3(FEVector3 &, FEVector3 &, float, FEVector3 *, FEVector3 *);
void FELerpQuaternion(FEQuaternion &, FEQuaternion &, float, FEQuaternion *, FEQuaternion *);
void FELerpColor(FEColor &, FEColor &, float, FEColor *, FEColor *);

#endif
