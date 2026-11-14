// AI-assisted GR8E69 reconstruction; see docs/research/frontend-screens.md.
#ifdef DECOMP_COMPARE
#include "PackageHandlers.h"
#include "Shell.h"
#include "GameSettings.h"
void ShellManager::SetStartMPDataFromShell() {
    g_GameSettings.profile->field_32074 &= ~0x40000000;
    g_GameSettings.profile->field_32074 |= 0x08000000;
    g_GameSettings.profile->unknown_3201c[2] = 5;
    g_GameSettings.profile->unknown_3201c[0] = 2;
    g_GameFlow.startingMP = 1;
    if (mpPlayerCount) g_GameSettings.profile->unknown_321e5 = mpPlayerCount;
}
#endif
