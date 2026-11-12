// AI-assisted GR8E69 reconstruction; see docs/research/feng-lifecycle.md.
#ifdef DECOMP_COMPARE
#include "FEngLists.h"
// The pending list destructor observes a separately owned string at +0x10.
struct FEPendingNodeView : FEMinNode { unsigned long unknown_c; char *text; };
FEPackage::FEPackage() {
    controlMask = 255;
    executing = unknown_18 = unknown_1c = unknown_20 = priority = 0;
    fileName = 0; parent = 0; unknown_40 = 0;
    resourceCount = 0; resourceRequests = 0;
    messageTargetCount = 0; messageTargets = 0;
    currentButton = 0; libraryRefs = 0;
    groupId = -1; resourceNames = 0; libraryRefCount = 0;
}
FEPackage::~FEPackage() {
    if (fileName) delete[] fileName;
    if (resourceRequests) delete[] resourceRequests;
    if (messageTargets) delete[] messageTargets;
    if (resourceNames) delete[] resourceNames;
    FEPendingNodeView *node;
    while ((node = (FEPendingNodeView *)pendingList.RemHead()) != 0) {
        if (node->text) delete[] node->text;
        delete node;
    }
    if (libraryRefs) delete[] libraryRefs;
}
#endif
