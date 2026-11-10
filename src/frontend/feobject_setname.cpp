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

void FEObject::SetName(const char *text) {
    if (name) {
        delete[] name;
        name = 0;
    }
    hash = (unsigned long)-1;
    if (text) {
        name = new ("source/FEngine/FEObject.cpp:197") char[strlen(text) + 1];
        strcpy(name, text);
        hash = FEHashUpper(name);
    }
}
