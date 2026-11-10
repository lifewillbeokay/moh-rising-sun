// ShellManager's short forwarding methods; see docs/Frontend.md.
#include "ShellManager.h"
#include <string.h>
#include "Shell.h"
extern CInputManager g_inputMgr;
struct BSObject;
void BSObjectTriggerEvent(BSObject *, unsigned short, void *, BSObject *, bool);
void DebugMsg(const char *, ...);
extern "C" int strcasecmp(const char *, const char *);

unsigned char ShellManager::GetNumCoopPlayers() {
    unsigned char count = 0;
    for (unsigned char i = 0; i <= 3; i++) count += (coopJoinBits >> i) & 1;
    return count;
}
