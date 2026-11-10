// PackageHandler: common helpers for the shell's screens; see docs/Frontend.md.
#include "FEngObjects.h"
#include "GameSettings.h"
#include <stdio.h>
extern "C" int strcasecmp(const char *, const char *);
void DebugMsg(const char *, ...);

void PackageHandler::EnableButton(FEObject *object) { ((FEStringObject *)object)->disabled = 0; }
void PackageHandler::DisableButton(FEObject *object) { ((FEStringObject *)object)->disabled = 1; }
void PackageHandler::EnableButton(FEPackage *package, const char *name) {
    FEObject *object = package->FindObjectByName(name);
    if (!object) DebugMsg("Unmatched Feng object:%s", name);
    else EnableButton(object);
}
void PackageHandler::DisableButton(FEPackage *package, const char *name) {
    FEObject *object = package->FindObjectByName(name);
    if (!object) DebugMsg("Unmatched Feng object:%s", name);
    else DisableButton(object);
}
