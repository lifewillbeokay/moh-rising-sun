// A fragment of the original compartment code. Names come from the symbol table; member names are
// inferred from this code.
#include "CompartmentInstance.h"

void CCompartment::CInstanceModel::SetModelMatrix(const MATRIX4 &matrix) {
    unsigned int count = models.size();
    for (unsigned int i = 0; i < count; i++)
        models[i]->transform = matrix;
}

void CCompartment::CInstanceModel::GetBoundingSphere(COORD3 &center, float &radius) {
    EAGL::Model *model = models[0];
    center.x = model->boundCenter.x;
    center.y = model->boundCenter.y;
    center.z = model->boundCenter.z;
    radius = model->boundRadius;
}
