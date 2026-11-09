// AI-assisted reconstruction using CC0 Frontline references and GR8E69
// evidence. Provenance and validation:
// docs/research/collision-distance-traversal.md.

#include "CDBTriangle.h"
#include "CollisionMath.h"

#ifdef DECOMP_COMPARE
CVector3 CCDBTriangle::GetNearestPoint(const CVector3 &point, float *outS,
                                       float *outT) const {
  CVector3 e0;
  CVector3 e1;
  CVector3 diff;
  float a01, a00, b1, b0, a11;
  e0.x = b.x - a.x;
  e0.y = b.y - a.y;
  e0.z = b.z - a.z;
  e1.x = c.x - a.x;
  e1.y = c.y - a.y;
  e1.z = c.z - a.z;
  diff.y = a.y - point.y;
  diff.x = a.x - point.x;
  diff.z = a.z - point.z;
  a01 = e1.x * e0.x + e1.y * e0.y + e1.z * e0.z;
  b1 = diff.x * e1.x + diff.y * e1.y + diff.z * e1.z;
  a00 = e0.x * e0.x + e0.y * e0.y + e0.z * e0.z;
  b0 = diff.x * e0.x + diff.y * e0.y + diff.z * e0.z;
  a11 = e1.x * e1.x + e1.y * e1.y + e1.z * e1.z;
  float det = fabs(a00 * a11 - a01 * a01);
  float s = a01 * b1 - a11 * b0;
  float t = a01 * b0 - a00 * b1;

  if (s + t <= det) {
    if (s < 0.0f) {
      if (t < 0.0f) {
        if (b0 < 0.0f) {
          t = 0.0f;
          if (-b0 >= a00)
            s = 1.0f;
          else
            s = -b0 / a00;
        } else {
          s = 0.0f;
          if (b1 >= 0.0f)
            t = 0.0f;
          else if (-b1 >= a11)
            t = 1.0f;
          else
            t = -b1 / a11;
        }
      } else {
        s = 0.0f;
        if (b1 >= 0.0f)
          t = 0.0f;
        else if (-b1 >= a11)
          t = 1.0f;
        else
          t = -b1 / a11;
      }
    } else if (t < 0.0f) {
      t = 0.0f;
      if (b0 >= 0.0f)
        s = 0.0f;
      else if (-b0 >= a00)
        s = 1.0f;
      else
        s = -b0 / a00;
    } else {
      float inverse = 1.0f / det;
      s *= inverse;
      t *= inverse;
    }
  } else {
    if (s < 0.0f) {
      float tmp0 = a01 + b0;
      float tmp1 = a11 + b1;
      if (tmp1 > tmp0) {
        float numer = tmp1 - tmp0;
        float denom = a00 - 2.0f * a01 + a11;
        if (numer >= denom) {
          s = 1.0f;
          t = 0.0f;
        } else {
          s = numer / denom;
          t = 1.0f - s;
        }
      } else {
        s = 0.0f;
        if (tmp1 <= 0.0f)
          t = 1.0f;
        else if (b1 >= 0.0f)
          t = 0.0f;
        else
          t = -b1 / a11;
      }
    } else if (t < 0.0f) {
      float tmp0 = a01 + b1;
      float tmp1 = a00 + b0;
      if (tmp1 > tmp0) {
        float numer = tmp1 - tmp0;
        float denom = a00 - 2.0f * a01 + a11;
        if (numer >= denom) {
          t = 1.0f;
          s = 0.0f;
        } else {
          t = numer / denom;
          s = 1.0f - t;
        }
      } else {
        t = 0.0f;
        if (tmp1 <= 0.0f)
          s = 1.0f;
        else if (b0 >= 0.0f)
          s = 0.0f;
        else
          s = -b0 / a00;
      }
    } else {
      float numer = a11 + b1 - a01 - b0;
      if (numer <= 0.0f) {
        s = 0.0f;
        t = 1.0f;
      } else {
        float denom = a00 - 2.0f * a01 + a11;
        if (numer >= denom) {
          s = 1.0f;
          t = 0.0f;
        } else {
          s = numer / denom;
          t = 1.0f - s;
        }
      }
    }
  }

  CVector3 result;
  float sx = s * e0.x;
  float sy = s * e0.y;
  float sz = s * e0.z;
  result.x = a.x + sx;
  result.y = a.y + sy;
  result.z = a.z + sz;
  float tx = t * e1.x;
  float ty = t * e1.y;
  float tz = t * e1.z;
  result.x = result.x + tx;
  result.y = result.y + ty;
  result.z = result.z + tz;
  if (outS)
    *outS = s;
  if (outT)
    *outT = t;
  return result;
}

#endif
