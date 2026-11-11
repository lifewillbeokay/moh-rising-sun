// AI-assisted GR8E69 reconstruction; see docs/research/feng-packages.md.
#include "FEngine.h"
#include "ShellManager.h"
void FEngine::UnloadLibraryPackage(FEPackage *package) {
    if (game->UnloadUnreferencedLibrary(package)) {
        RemoveFromLibraryList(package);
        int release = game ? game->PackageWillUnload(package) : 1;
        package->Shutdown(game);
        if (release && package) delete package;
    }
}
