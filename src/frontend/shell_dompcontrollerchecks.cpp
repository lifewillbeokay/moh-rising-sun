// ShellManager's short forwarding methods; see docs/Frontend.md.
#include "ShellManager.h"
#include <string.h>
#include "Shell.h"
extern CInputManager g_inputMgr;
struct BSObject;
void BSObjectTriggerEvent(BSObject *, unsigned short, void *, BSObject *, bool);
void DebugMsg(const char *, ...);
extern "C" int strcasecmp(const char *, const char *);

void ShellManager::DoMPControllerChecks() {}
void ShellManager::RemoveMPViewPorts() {
    if (engine) {
        FEPackageList *list = (FEPackageList *)&engine->packages;
        if (list) {
            unsigned long viewport = FEHashUpper("MPJoin.fng");
            unsigned long split = FEHashUpper("MPWaitingJoin.fng");
            for (int i = 0; i < (unsigned char)g_inputMgr.GetNumPorts(); i++) {
                FEPackage *p = list->FindPackage((unsigned char)i);
                if (p && (p->hash == viewport || p->hash == split)) engine->QueuePackagePop(p);
            }
        }
    }
}
void ShellManager::ResetActiveControllerIndicies() {
    for (int i = 0; i < 4; i++) activeControllers[i] = -1;
}
