// FEPackage.cpp: a package's objects, resources and library references.
#include "FEngObjects.h"
#include "FEMessageResponse.h"
#include <string.h>

void FEPackage::Update(FEngine *owner, long time) {
    engine = owner;
    lastDeltaTime = time;
    for (FEObject *object = (FEObject *)objectList.head; object; object = (FEObject *)object->next) UpdateObject(object, time);
}
// Two message hashes, each sent to the package and to the button: one for the old button, one for the new.
// Reading them as focus lost and focus gained is inferred from this use; the hash names are unknown.
void FEPackage::SetCurrentButton(FEngine *owner, FEObject *button, bool notify) {
    if (notify) {
        if (currentButton) {
            owner->QueueMessage(0x55d1e635, 0, this, currentButton, 0);
            owner->QueueMessage(0x55d1e635, currentButton, this, (FEObject *)-5, 0);
        }
        if (button) {
            owner->QueueMessage(0xabc08912, 0, this, button, 0);
            owner->QueueMessage(0xabc08912, button, this, (FEObject *)-5, 0);
        }
    }
    currentButton = button;
}
int ResourceConnector::Callback(FEObject *object) {
    switch (object->type) {
    case 4:
        ConnectListBoxResources((FEListBox *)object);
        break;
    case 5:
    case 6:
        break;
    default:
        if (object->resourceIndex != 0xffff) {
            object->resourceHandle = (*requests)[object->resourceIndex].data;
            object->resourceData = (*requests)[object->resourceIndex].handle;
        }
        break;
    }
    return 1;
}
