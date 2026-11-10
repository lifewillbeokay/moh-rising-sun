// CCompartment::CInstanceModel with its real vectors, kept apart from Compartment.h: the STL headers'
// inlines own .rodata strings that fragments with no literals must not emit, and the placement new in
// Compartment.h's includes conflicts with <new>. The class name comes from the symbol table and the
// two vectors from their reserve instantiations; other names are inferred.
#ifndef GAME_COMPARTMENT_INSTANCE_H
#define GAME_COMPARTMENT_INSTANCE_H
#include "profile.h"
#include <vector>
#include "EAGLTransform.h"
#include "ResourceTypes.h"

namespace EAGL {
class Model {
  public:
    unsigned char unknown00[12];
    MATRIX4 transform; // +0x0c
    unsigned char unknown4c[140 - 76];
    COORD3 boundCenter; // +0x8c
    float boundRadius;  // +0x98
};
}

class CCompartment {
  public:
    class CInstanceModel;
};
class CCompartment::CInstanceModel {
  public:
    EAGL::Model *GetVariation(int);
    void ReserveModelSpace(int);
    void DeleteLoaders();
    void SetModelMatrix(const MATRIX4 &);
    void GetBoundingSphere(COORD3 &, float &);
    ~CInstanceModel();
    _STL::vector<EAGL::Model *> models;
    _STL::vector<EALA::Resource::Pointer<EALA::EAGLLoader> > loaders; // +0x10
};
#endif
