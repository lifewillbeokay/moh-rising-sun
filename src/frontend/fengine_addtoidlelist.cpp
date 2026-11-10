// FEngine's list and marker helpers; see docs/Frontend.md.
#include "FEngine.h"
#include <string.h>

void FEngine::AddToIdleList(FEPackage *package) { idlePackages.AddTail(package); }
FEPackage *FEngine::FindIdlePackage(const char *name) const { return (FEPackage *)idlePackages.FindNode(name); }
FEPackage *FEngine::GetFirstLibrary() const { return (FEPackage *)libraries.head; }
void FEngine::AddToLibraryList(FEPackage *package) { libraries.AddTail(package); }
void FEngine::RemoveFromLibraryList(FEPackage *package) { libraries.RemNode(package); }
FEPackage *FEngine::FindLibraryPackage(unsigned long hash) const {
    for (FEPackage *p = GetFirstLibrary(); p; p = (FEPackage *)p->next) {
        if (FEHashUpper(p->fileName + 2) == hash) return p;
    }
    return 0;
}
FEObject *FEngine::FindObject(unsigned long hash, const char *packageName) const {
    FEPackage *p;
    if (packageName) {
        p = (FEPackage *)packages.FindNode(packageName);
        if (p) return p->FindObjectByHash(hash);
    } else {
        for (p = (FEPackage *)packages.head; p; p = (FEPackage *)p->next) {
            FEObject *object = p->FindObjectByHash(hash);
            if (object) return object;
        }
    }
    return 0;
}
FEObject *FEngine::FindObject(const char *name, const char *packageName) const { return FindObject(FEHashUpper(name), packageName); }
void FEngine::StartCheckMovieDemoScreen() { checkMovieDemoScreen = 0; }
