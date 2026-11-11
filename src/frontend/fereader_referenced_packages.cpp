// AI-assisted GR8E69 reconstruction; see docs/research/feng-packages.md.
#ifdef DECOMP_COMPARE
#include "FEPackageReader.h"
#include "ShellManager.h"
int FEPackageReader::ReadReferencedPackagesChunk() {
    FEChunk *chunk = FindChild(root, 0x4c62694c);
    if (chunk) {
        unsigned long *body = (unsigned long *)(chunk + 1);
        unsigned long count = FESwap(*body), *offset = body + 1;
        FEPackage *owner = package;
        for (unsigned long i = 0; i < count; ++i) {
            FENode *name = new FENode;
            name->SetName((char *)body + FESwap(*offset++));
            owner->referencedPackages.AddNode(owner->referencedPackages.tail, name);
        }
        for (FENode *name = (FENode *)owner->referencedPackages.head; name; name = (FENode *)name->next) {
            FEPackage *library = engine->FindLibraryPackage(name->hash);
            if (library) ++library->libraryUseCount;
            else {
                unsigned char *allocation;
                bool release;
                int data = game->GetPackageData(name->name, &allocation, release);
                if (!data) return 0;
                library = engine->LoadPackage((const void *)data, true);
                if (release && allocation) delete[] allocation;
                if (!library) return 0;
                game->PackageWasLoaded(library);
                library->libraryUseCount = 1;
                engine->AddToLibraryList(library);
            }
        }
    }
    return 1;
}
#endif
