// ShellManager's short forwarding methods; see docs/Frontend.md.
#include "ShellManager.h"
#include <string.h>
#include "Shell.h"
extern CInputManager g_inputMgr;
struct BSObject;
void BSObjectTriggerEvent(BSObject *, unsigned short, void *, BSObject *, bool);
void DebugMsg(const char *, ...);
extern "C" int strcasecmp(const char *, const char *);

void ShellManager::DoCoopControllerChecks() {}
void ShellManager::ResetCoopJoinInfo(signed char index) {
    if (index == -1) {
        for (unsigned char i = 0; i <= 1; i++) {
            coopJoin[i].package = 0;
            coopJoin[i].port = -1;
            coopJoin[i].mask = 0;
        }
    } else {
        coopJoin[index].package = 0;
        coopJoin[index].port = -1;
        coopJoin[index].mask = 0;
    }
}
