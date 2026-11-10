// FEObject's small methods; see docs/Frontend.md.
#include "FEngObjects.h"
class FEMultiPool {
  public:
    unsigned char *Alloc(unsigned long);
    void Free(unsigned char *);
};
extern FEMultiPool ObjDataPool;
class FEMessageResponse : public FEMinNode {
  public:
    unsigned long message; // +0xc
};

void FEObject::SetDataSize(unsigned long size) {
    ObjDataPool.Free(data);
    data = 0;
    data = ObjDataPool.Alloc(size);
    dataSize = size;
}
