// FEPackageReader.cpp: the chunk readers.
#include "FEPackageReader.h"
#include "FEngine.h"
#include "FEngStrings.h"
#include "FEListBox.h"
#include "FEngElements.h"
#include "FEScript.h"
#include "ShellManager.h"
#include <string.h>
inline void *operator new(unsigned int size, const char *label) { return DWI_alloc(label, size, 256); }
void FEKeyInterp(FEScript *, unsigned char, long, FEObject *);

FEPackage *FEPackageReader::Load(const void *data, FEGameInterface *gameInterface, FEngine *feng, bool library, bool keep, bool noInterp) {
    FEPackage *result = 0;
    Reset();
    skipInterp = noInterp;
    isLibrary = library;
    keepExisting = keep;
    root = (FEChunk *)data;
    game = gameInterface;
    engine = feng;
    if (ReadHeaderChunk() && ReadTypeSizes() && ReadReferencedPackagesChunk() && ReadLibraryRefsChunk() && ReadResourceChunk() && ReadObjectChunk() && ReadPackageResponseChunk() && ReadMessageTargetListChunk()) {
        package->unknown_1c = skipInterp;
        if (package->Startup(game)) {
            result = package;
            package = 0;
        }
    }
    delete package;
    package = 0;
    return result;
}
