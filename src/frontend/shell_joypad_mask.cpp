// AI-assisted reconstruction from GR8E69; see docs/research/frontend-popup-list.md.
#include "ShellManager.h"
#include "Shell.h"
struct ShellPadFlag { unsigned short unknown_0, key; int mask; };
extern ShellPadFlag padFlags[];
#ifdef DECOMP_COMPARE
unsigned long ShellManager::GetJoyPadMask(unsigned char pad) {
    if (g_inputMgr.badControllerSet) return 0;
    int mask = 0;
    if (!g_inputMgr.IsReady((signed char)pad)) return 0;
    for (int i = 0; i < 9; ++i) {
        if (g_inputMgr.GetValue(pad, padFlags[i].key)) { mask = padFlags[i].mask; break; }
    }
    int x = g_inputMgr.GetValue(pad, 0x81);
    int y = g_inputMgr.GetValue(pad, 0x82);
    if (!mask) {
        if (x < -63) mask = 4;
        else if (x > 63) mask = 8;
        else if (y < -63) mask = 1;
        else if (y > 63) mask = 2;
    }
    lastStickY = y;
    lastStickX = x;
    return mask;
}
#endif
