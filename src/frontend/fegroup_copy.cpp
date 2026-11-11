// AI-assisted GR8E69 reconstruction; see docs/research/feng-packages.md.
#ifdef DECOMP_COMPARE
#include "FEngLists.h"
FEGroup::FEGroup(const FEGroup &other, bool children, bool referenceScripts)
    : FEObject(other, referenceScripts) {
    if (children) {
        for (FEObject *node = (FEObject *)other.childList.head; node; node = (FEObject *)node->next)
            childList.AddNode(childList.tail, node->Clone(referenceScripts));
    }
}
#endif
