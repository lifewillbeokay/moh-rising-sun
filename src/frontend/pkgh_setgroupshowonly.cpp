// PackageHandler: common helpers for the shell's screens; see docs/Frontend.md.
#include "FEngObjects.h"
#include "GameSettings.h"
#include <stdio.h>
extern "C" int strcasecmp(const char *, const char *);
void DebugMsg(const char *, ...);

int PackageHandler::SetGroupShowOnly(FEGroup *group, const char *name) { return SetGroupShowOnly(group, FEHashUpper(name)); }
int PackageHandler::SetGroupShowOnly(unsigned long groupHash, unsigned long hash) {
    FEGroup *group = (FEGroup *)package->FindObjectByHash(groupHash);
    int result;
    if (group) result = SetGroupShowOnly(group, hash);
    else result = -3;
    return result;
}
void PackageHandler::QueueMessage(unsigned int message) { engine->QueueMessage(message, 0, package, (FEObject *)0xfffffffd, 0); }
void PackageHandler::HandleTick() {}
