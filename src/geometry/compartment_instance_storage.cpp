// A fragment of the original compartment code: instance model lookup and storage. Names come from the
// symbol table; member names are inferred from this code.
#include "CompartmentInstance.h"

EAGL::Model *CCompartment::CInstanceModel::GetVariation(int index) {
    if ((unsigned int)index >= models.size())
        return models[0];
    return models[index];
}

void CCompartment::CInstanceModel::ReserveModelSpace(int count) {
    models.reserve(count);
    loaders.reserve(count);
}

void CCompartment::CInstanceModel::DeleteLoaders() {
    loaders.clear();
}
