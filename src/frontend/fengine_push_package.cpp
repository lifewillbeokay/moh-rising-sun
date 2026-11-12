// AI-assisted GR8E69 reconstruction; see docs/research/feng-lifecycle.md.
#include "FEngine.h"
#include "ShellManager.h"
#include <string.h>
FEPackage *FEngine::PushPackage(const char *name, unsigned char priority, unsigned long mask) {
    FEPackage *package = FindIdlePackage(name);
    if (!package) {
        signed char count = strlen(name);
        const char *base = name + count - 1;
        while (*base != '/' && *base != '\\' && count > 0) { --base; --count; }
        if (count) ++base;
        package = FindIdlePackage(base);
    }
    if (package) {
        PackageInitStateCB callback;
        package->unknown_18 = 1;
        package->ForAllObjects(callback);
        idlePackages.RemNode(package);
    } else {
        unsigned char *allocation;
        bool release;
        int data = game->GetPackageData(name, &allocation, release);
        if (!data) return 0;
        package = LoadPackage((const void *)data, false);
        if (game->LocalizePackage(package) < 0) return 0;
        if (release && allocation) delete[] allocation;
        if (!package) return 0;
    }
    package->controlMask = mask; package->priority = priority; package->executing = executing;
    if (game) game->PackageWasLoaded(package);
    ((FEPackageList *)&packages)->AddPackage(package);
    return package;
}
