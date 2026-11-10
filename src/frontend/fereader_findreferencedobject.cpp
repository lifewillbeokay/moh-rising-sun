// FEPackageReader.cpp: object tags.
#include "FEPackageReader.h"
#include "FEngine.h"
#include "FEngStrings.h"
#include "FEngElements.h"
#include "FEScript.h"
#include "FEMessageResponse.h"

int FEPackageReader::FindReferencedObject(unsigned long guid, FEObject **foundObject, FEPackage **foundPackage) {
    *foundObject = 0;
    *foundPackage = 0;
    const FELibraryReference *reference = package->FindLibraryReference(guid);
    if (!reference) return 0;
    *foundPackage = engine->FindLibraryPackage(reference->unknown_4);
    if (!*foundPackage) return 0;
    *foundObject = (*foundPackage)->FindObjectByGUID(reference->unknown_8);
    return *foundObject != 0;
}
