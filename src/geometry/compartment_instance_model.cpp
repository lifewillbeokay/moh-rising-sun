// A fragment of the original compartment code: instance model lookup. Names come from the symbol table; member names
// are inferred from this code.
#include "Compartment.h"

class CCompartment::CInstanceModel {
  public:
    EAGL::Model *GetVariation(int);
    unsigned char unknown00[32];
};

EAGL::Model *CCompartment::GetInstanceModel(int index, int variation) {
    if (GetInstanceType(index) == 1)
        return (instanceModels.start)[index].GetVariation(variation);
    return (instanceModels.start)[index].GetVariation(0);
}
