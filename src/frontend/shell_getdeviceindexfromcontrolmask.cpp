// ShellManager's short forwarding methods; see docs/Frontend.md.
#include "ShellManager.h"
#include <string.h>
#include "Shell.h"
extern CInputManager g_inputMgr;
struct BSObject;
void BSObjectTriggerEvent(BSObject *, unsigned short, void *, BSObject *, bool);
void DebugMsg(const char *, ...);
extern "C" int strcasecmp(const char *, const char *);

signed char ShellManager::GetDeviceIndexFromControlMask(unsigned long mask) {
    signed char index = mask >> 1;
    if (index == 4) index = 3;
    return index;
}
signed char ShellManager::GetPortNumFromControlMask(unsigned long mask) {
    return g_inputMgr.GetPortNum(GetDeviceIndexFromControlMask(mask));
}
void ShellManager::SetKeyboardString(const char *text) { strncpy(keyboardString, text, 17); }
