// FEPackage.cpp: a package's objects, resources and library references.
#include "FEngObjects.h"
#include "FEMessageResponse.h"
#include <string.h>

int PackageInitStateCB::Callback(FEObject *object) {
    object->SetCurrentScript(object->FindScript(0x1744b3));
    object->currentScript->time = 0;
    object->unknownBit6 = 7;
    object->changed = 1;
    return 1;
}
