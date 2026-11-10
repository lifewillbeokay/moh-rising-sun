// FEObject.cpp: an object's construction, name and keyed data layout.
#include "FEngObjects.h"
#include <string.h>
class FEMultiPool {
  public:
    unsigned char *Alloc(unsigned long);
    void Free(unsigned char *);
};
extern FEMultiPool ObjDataPool;
inline void *operator new(unsigned int, void *p) { return p; }

long FEObject::GetDataOffset(FEKeyTrack_Indices index) {
    switch (index) {
    case 0: return 0;
    case 1: return 16;
    case 2: return 28;
    case 3: return 40;
    case 4: return 56;
    case 5: return 68;
    case 6: return 76;
    case 7: return 84;
    case 8: return 100;
    case 9: return 116;
    case 10: return 132;
    }
    return 0;
}
FEObject *FEObject::Clone(bool deep) {
    return new (DWI_alloc("source/FEngine/FEObject.cpp:633", sizeof(FEObject), 256)) FEObject(*this, deep);
}
