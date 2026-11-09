#ifndef GAME_CTRIANGLE_H
#define GAME_CTRIANGLE_H
#include "CVector3.h"
// Observed three-vertex prefix, not a complete allocation contract.
class CTriangle {
  public:
    CVector3 a, b, c;
};
#endif
