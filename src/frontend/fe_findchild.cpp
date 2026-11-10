// FEGroup's child lookups by hash and by name.
#include "FEngLists.h"

FEObject *FEGroup::FindChild(unsigned long hash) const {
    FEObject *child = (FEObject *)childList.head;
    while (child && child->hash != hash) child = (FEObject *)child->next;
    return child;
}
FEObject *FEGroup::FindChild(const char *name) const { return FindChild(FEHashUpper(name)); }
