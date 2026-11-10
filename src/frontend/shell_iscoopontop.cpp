// ShellManager's short forwarding methods; see docs/Frontend.md.
#include "ShellManager.h"
#include <string.h>
#include "Shell.h"
extern CInputManager g_inputMgr;
struct BSObject;
void BSObjectTriggerEvent(BSObject *, unsigned short, void *, BSObject *, bool);
void DebugMsg(const char *, ...);
extern "C" int strcasecmp(const char *, const char *);

int ShellManager::IsCoopOnTop(unsigned char mask) {
    unsigned char port = GetPortNumFromControlMask(mask);
    return coopJoin[0].port == port;
}
int ShellManager::IsCoopOnTopFromPortNum(unsigned char port) { return coopJoin[0].port == port; }
