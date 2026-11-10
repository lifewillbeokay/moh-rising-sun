// ShellManager: co-op joining, controller indices and starting a multiplayer game.
#include "GameSettingsViews.h"
#include "Shell.h"

void ShellManager::Reset(const char *screen) {
    int popped = 0;
    FEPackage *package = (FEPackage *)engine->packages.head;
    if (package) {
        popped = 1;
        for (; package; package = (FEPackage *)package->next) engine->QueuePackagePop(package);
    }
    if (popped) Process(20);
    if (screen) engine->QueuePackagePush(screen, 0, 255);
    engine->resetRequested = 1;
}
