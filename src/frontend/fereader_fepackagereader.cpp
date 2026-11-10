// FEPackageReader: chunk readers; see docs/Frontend.md.
#include "FEPackageReader.h"

FEPackageReader::FEPackageReader() { Reset(); }
FEPackageReader::~FEPackageReader() {}
void FEPackageReader::Reset() {
    root = 0;
    package = 0;
    object = 0;
    parentGroup = 0;
    parent = 0;
    typeCount = 0;
    types = 0;
    scriptCount = 0;
    objectCount = 0;
    buttonIndex = 0;
    buttonCount = 0;
}
