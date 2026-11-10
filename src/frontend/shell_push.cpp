// ShellManager's short forwarding methods; see docs/Frontend.md.
#include "ShellManager.h"
#include <string.h>
#include "Shell.h"
extern CInputManager g_inputMgr;
struct BSObject;
void BSObjectTriggerEvent(BSObject *, unsigned short, void *, BSObject *, bool);
void DebugMsg(const char *, ...);
extern "C" int strcasecmp(const char *, const char *);

void ShellManager::Push(const char *name) {
    if (name) engine->QueuePackagePush(name, 0, 255);
}
void ShellManager::Swap(const char *name, FEPackage *package) { engine->QueuePackageSwitch(name, package); }
