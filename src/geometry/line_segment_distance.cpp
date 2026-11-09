// AI-assisted reconstruction using CC0 Frontline references and GR8E69
// evidence. Provenance and validation:
// docs/research/collision-distance-traversal.md.

#include "CLine3.h"
#include "CollisionMath.h"

#ifdef DECOMP_COMPARE
bool CLine3::GetSquaredDistanceBetweenSegments(const CLine3 &other, float &fS,
                                               float &fT,
                                               float *pfSqrDist) const {
  CVector3 kDiff;
  kDiff.x = start.x - other.start.x;
  kDiff.y = start.y - other.start.y;
  kDiff.z = start.z - other.start.z;
  float fA00 = CachedLengthSquared();
  float fA01 = -CollisionDot(direction, other.direction);
  float fA11 = other.CachedLengthSquared();
  float fB0 = CollisionDot(direction, kDiff);
  float fB1 = -CollisionDot(other.direction, kDiff);
  float fC = CollisionDot(kDiff, kDiff);
  float fDet = fabs(fA00 * fA11 - fA01 * fA01);
  float fSqrDist;
  bool parallel;
  float fTmp;

  parallel = fDet < 1e-5f;
  if (parallel == false) {
    fS = fA01 * fB1 - fA11 * fB0;
    fT = fA01 * fB0 - fA00 * fB1;

    if (fS >= 0.0f) {
      if (fS <= fDet) {
        if (fT >= 0.0f) {
          if (fT <= fDet) {
            float fInvDet = 1.0f / fDet;
            fS *= fInvDet;
            fT *= fInvDet;
            fSqrDist = fS * (fA00 * fS + fA01 * fT + 2.0f * fB0) +
                       fT * (fA01 * fS + fA11 * fT + 2.0f * fB1) + fC;
          } else {
            fT = 1.0f;
            fTmp = fA01 + fB0;
            if (fTmp >= 0.0f) {
              fS = 0.0f;
              fSqrDist = fA11 + 2.0f * fB1 + fC;
            } else if (-fTmp >= fA00) {
              fS = 1.0f;
              fSqrDist = fA00 + fA11 + fC + 2.0f * (fB1 + fTmp);
            } else {
              fS = -fTmp / fA00;
              fSqrDist = fTmp * fS + fA11 + 2.0f * fB1 + fC;
            }
          }
        } else {
          fT = 0.0f;
          if (fB0 >= 0.0f) {
            fS = 0.0f;
            fSqrDist = fC;
          } else if (-fB0 >= fA00) {
            fS = 1.0f;
            fSqrDist = fA00 + 2.0f * fB0 + fC;
          } else {
            fS = -fB0 / fA00;
            fSqrDist = fB0 * fS + fC;
          }
        }
      } else {
        if (fT >= 0.0f) {
          if (fT <= fDet) {
            fS = 1.0f;
            fTmp = fA01 + fB1;
            if (fTmp >= 0.0f) {
              fT = 0.0f;
              fSqrDist = fA00 + 2.0f * fB0 + fC;
            } else if (-fTmp >= fA11) {
              fT = 1.0f;
              fSqrDist = fA00 + fA11 + fC + 2.0f * (fB0 + fTmp);
            } else {
              fT = -fTmp / fA11;
              fSqrDist = fTmp * fT + fA00 + 2.0f * fB0 + fC;
            }
          } else {
            fTmp = fA01 + fB0;
            if (-fTmp <= fA00) {
              fT = 1.0f;
              if (fTmp >= 0.0f) {
                fS = 0.0f;
                fSqrDist = fA11 + 2.0f * fB1 + fC;
              } else {
                fS = -fTmp / fA00;
                fSqrDist = fTmp * fS + fA11 + 2.0f * fB1 + fC;
              }
            } else {
              fS = 1.0f;
              fTmp = fA01 + fB1;
              if (fTmp >= 0.0f) {
                fT = 0.0f;
                fSqrDist = fA00 + 2.0f * fB0 + fC;
              } else if (-fTmp >= fA11) {
                fT = 1.0f;
                fSqrDist = fA00 + fA11 + fC + 2.0f * (fB0 + fTmp);
              } else {
                fT = -fTmp / fA11;
                fSqrDist = fTmp * fT + fA00 + 2.0f * fB0 + fC;
              }
            }
          }
        } else {
          if (-fB0 < fA00) {
            fT = 0.0f;
            if (fB0 >= 0.0f) {
              fS = 0.0f;
              fSqrDist = fC;
            } else {
              fS = -fB0 / fA00;
              fSqrDist = fB0 * fS + fC;
            }
          } else {
            fS = 1.0f;
            fTmp = fA01 + fB1;
            if (fTmp >= 0.0f) {
              fT = 0.0f;
              fSqrDist = fA00 + 2.0f * fB0 + fC;
            } else if (-fTmp >= fA11) {
              fT = 1.0f;
              fSqrDist = fA00 + fA11 + fC + 2.0f * (fB0 + fTmp);
            } else {
              fT = -fTmp / fA11;
              fSqrDist = fTmp * fT + fA00 + 2.0f * fB0 + fC;
            }
          }
        }
      }
    } else {
      if (fT >= 0.0f) {
        if (fT <= fDet) {
          fS = 0.0f;
          if (fB1 >= 0.0f) {
            fT = 0.0f;
            fSqrDist = fC;
          } else if (-fB1 >= fA11) {
            fT = 1.0f;
            fSqrDist = fA11 + 2.0f * fB1 + fC;
          } else {
            fT = -fB1 / fA11;
            fSqrDist = fB1 * fT + fC;
          }
        } else {
          fTmp = fA01 + fB0;
          if (fTmp < 0.0f) {
            fT = 1.0f;
            if (-fTmp >= fA00) {
              fS = 1.0f;
              fSqrDist = fA00 + fA11 + fC + 2.0f * (fB1 + fTmp);
            } else {
              fS = -fTmp / fA00;
              fSqrDist = fTmp * fS + fA11 + 2.0f * fB1 + fC;
            }
          } else {
            fS = 0.0f;
            if (fB1 >= 0.0f) {
              fT = 0.0f;
              fSqrDist = fC;
            } else if (-fB1 >= fA11) {
              fT = 1.0f;
              fSqrDist = fA11 + 2.0f * fB1 + fC;
            } else {
              fT = -fB1 / fA11;
              fSqrDist = fB1 * fT + fC;
            }
          }
        }
      } else {
        if (fB0 < 0.0f) {
          fT = 0.0f;
          if (-fB0 >= fA00) {
            fS = 1.0f;
            fSqrDist = fA00 + 2.0f * fB0 + fC;
          } else {
            fS = -fB0 / fA00;
            fSqrDist = fB0 * fS + fC;
          }
        } else {
          fS = 0.0f;
          if (fB1 >= 0.0f) {
            fT = 0.0f;
            fSqrDist = fC;
          } else if (-fB1 >= fA11) {
            fT = 1.0f;
            fSqrDist = fA11 + 2.0f * fB1 + fC;
          } else {
            fT = -fB1 / fA11;
            fSqrDist = fB1 * fT + fC;
          }
        }
      }
    }
  } else {
    if (fA01 > 0.0f) {
      if (fB0 >= 0.0f) {
        fS = 0.0f;
        fT = 0.0f;
        fSqrDist = fC;
      } else if (-fB0 <= fA00) {
        fS = -fB0 / fA00;
        fT = 0.0f;
        fSqrDist = fB0 * fS + fC;
      } else {
        fS = 1.0f;
        fTmp = fA00 + fB0;
        if (-fTmp >= fA01) {
          fT = 1.0f;
          fSqrDist = fA00 + fA11 + fC + 2.0f * (fA01 + fB0 + fB1);
        } else {
          fT = -fTmp / fA01;
          fSqrDist =
              fA00 + 2.0f * fB0 + fC + fT * (fA11 * fT + 2.0f * (fA01 + fB1));
        }
      }
    } else {
      if (-fB0 >= fA00) {
        fS = 1.0f;
        fT = 0.0f;
        fSqrDist = fA00 + 2.0f * fB0 + fC;
      } else if (fB0 <= 0.0f) {
        fS = -fB0 / fA00;
        fT = 0.0f;
        fSqrDist = fB0 * fS + fC;
      } else {
        fS = 0.0f;
        if (fB0 >= -fA01) {
          fT = 1.0f;
          fSqrDist = fA11 + 2.0f * fB1 + fC;
        } else {
          fT = -fB0 / fA01;
          fSqrDist = fC + fT * (2.0f * fB1 + fA11 * fT);
        }
      }
    }
  }
  if (pfSqrDist)
    *pfSqrDist = fabs(fSqrDist);
  return parallel;
}

#endif
