// PackageHandler: common helpers for the shell's screens; see docs/Frontend.md.
#include "FEngObjects.h"
#include "GameSettings.h"
#include <stdio.h>
extern "C" int strcasecmp(const char *, const char *);
void DebugMsg(const char *, ...);

void PackageHandler::EnableButton(FEPackage *package, unsigned long hash) {
    FEObject *object = package->FindObjectByHash(hash);
    if (!object) DebugMsg("Unmatched Feng object:%d", hash);
    else EnableButton(object);
}
void PackageHandler::DisableButton(FEPackage *package, unsigned long hash) {
    FEObject *object = package->FindObjectByHash(hash);
    if (!object) DebugMsg("Unmatched Feng object:%d", hash);
    else DisableButton(object);
}
