// AI-assisted reconstruction from GR8E69. Fragments of the original AILaggedVector.cpp;
// method names come from the symbol table, member names are inferred.
#include "CVector3.h"

class CAILaggedVector {
  public:
    void Init(float, float);
    void Reset(const CVector3 &, const CVector3 &);
    CVector3 value;   // +0x00
    CVector3 target;  // +0x10
    float rate;       // +0x20
    float lag;        // +0x24
};

void CAILaggedVector::Init(float newRate, float newLag) {
    rate = newRate;
    lag = newLag;
}

void CAILaggedVector::Reset(const CVector3 &newValue, const CVector3 &newTarget) {
    value = newValue;
    target = newTarget;
}
