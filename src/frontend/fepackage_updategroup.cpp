// FEPackage.cpp: a package's objects, resources and library references.
#include "FEngObjects.h"
#include "FEMessageResponse.h"
#include <string.h>

void FEPackage::UpdateGroup(FEGroup *group, long time) {
    for (FEObject *object = group->children; object; object = (FEObject *)object->next) {
        UpdateObject(object, time);
        object->flags |= group->flags & 0x03c00000;
    }
}
