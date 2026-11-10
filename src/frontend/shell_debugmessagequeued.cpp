// ShellManager's short forwarding methods; see docs/Frontend.md.
#include "ShellManager.h"
#include <string.h>
#include "Shell.h"
extern CInputManager g_inputMgr;
struct BSObject;
void BSObjectTriggerEvent(BSObject *, unsigned short, void *, BSObject *, bool);
void DebugMsg(const char *, ...);
extern "C" int strcasecmp(const char *, const char *);

void ShellManager::DebugMessageQueued(unsigned long message, FEObject *from, FEPackage *, FEObject *to, unsigned long mask) {
    DebugMsg("FEng Queued msg %X, from %X to %X, mask is %X\n", message, from, to, mask);
}
void ShellManager::DebugMessageProcessed(unsigned long message, FEObject *from, FEObject *to, FEPackage *, unsigned long mask) {
    DebugMsg("FEng Processed msg %X, from %X to %X, mask is %X\n", message, to, from, mask);
}
