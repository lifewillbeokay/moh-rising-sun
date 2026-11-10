// A fragment of the original compartment code: instance loaders. Names come from the symbol table; member names
// are inferred from this code.
#include "Compartment.h"

class CCompartment::CInstanceModel {
  public:
    void DeleteLoaders();
    unsigned char unknown00[32];
};

void CCompartment::DeleteInstanceLoaders() {
    unsigned int count = instanceModels.finish - instanceModels.start;
    for (unsigned int i = 0; i < count; i++)
        instanceModels.start[i].DeleteLoaders();
}
