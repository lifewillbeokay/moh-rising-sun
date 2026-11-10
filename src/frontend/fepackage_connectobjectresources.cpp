// FEPackage.cpp: a package's objects, resources and library references.
#include "FEngObjects.h"
#include "FEMessageResponse.h"
#include <string.h>

void FEPackage::ConnectObjectResources() {
    ResourceConnector connector;
    connector.package = this;
    connector.requests = &resourceRequests;
    ForAllObjects(connector);
}
void FEPackage::SetNumLibraryRefs(unsigned long count) {
    if (count == 0) {
        if (libraryRefs) delete[] libraryRefs;
        libraryRefs = (FELibraryReference *)count;
    } else {
        FELibraryReference *refs = new ("source/FEngine/FEPackage.cpp:1079") FELibraryReference[count];
        unsigned long keep = count;
        if (keep > libraryRefCount) keep = libraryRefCount;
        if (keep) {
            void *to = refs;
            void *from = libraryRefs;
            memcpy(to, from, keep * sizeof(FELibraryReference));
        }
        if (libraryRefs) delete[] libraryRefs;
        libraryRefs = refs;
        libraryRefCount = count;
    }
}
const FELibraryReference *FEPackage::FindLibraryReference(unsigned long id) const {
    for (unsigned long i = 0; i < libraryRefCount; i++) {
        if (libraryRefs[i].id == id) return &libraryRefs[i];
    }
    return 0;
}
