// PackageHandler: common helpers for the shell's screens; see docs/Frontend.md.
#include "FEngObjects.h"
#include "GameSettings.h"
#include <stdio.h>
extern "C" int strcasecmp(const char *, const char *);
void DebugMsg(const char *, ...);

int PackageHandler::SetScript(FEPackage *package, const char *name, const char *script) {
    FEStringObject *object = (FEStringObject *)package->FindObjectByName(name);
    if (!object) return -1;
    FEScript *found = object->FindScript(FEHashUpper(script));
    if (!found) return -2;
    object->SetCurrentScript(found);
    object->changed = 1;
    return 0;
}
int PackageHandler::SetScript(FEPackage *package, unsigned long hash, const char *script) {
    FEStringObject *object = (FEStringObject *)package->FindObjectByHash(hash);
    if (!object) return -1;
    FEScript *found = object->FindScript(FEHashUpper(script));
    if (!found) return -2;
    object->SetCurrentScript(found);
    object->changed = 1;
    return 0;
}
