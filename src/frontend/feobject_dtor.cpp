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

FEObject::~FEObject() {
    if (pDestructorCallback) pDestructorCallback->ObjectDestroyed(this);
    ObjDataPool.Free(data);
    if (name) delete[] name;
}
