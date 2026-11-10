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

int FEPackageReader::ReadHeaderChunk() {
    FEChunk *file = root;
    if (FESwap(file->id) != 0xe76e4546) return 0;
    FEChunk *head = file + 1;
    if (FESwap(head->id) != 0x64486b50) return 0;
    FEPackageHeader *header = (FEPackageHeader *)(head + 1);
    if (FESwap(header->version) <= 0x1ffff) return 0;
    package = new ("source/FEngine/FEPackageReader.cpp:188") FEPackage;
    package->currentButton = 0;
    const char *name = (const char *)file + 40;
    objectCount = FESwap(header->objectCount);
    scriptCount = FESwap(header->scriptCount);
    const char *fileName = name + FESwap(header->nameLength);
    package->SetName(name);
    package->SetFilename(fileName);
    return 1;
}
