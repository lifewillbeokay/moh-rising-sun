// AI-assisted GR8E69 reconstruction; see docs/research/frontend-screens.md.
#ifdef DECOMP_COMPARE
#include "ShellManager.h"
#include "Shell.h"
#include "GameSettings.h"
void ShellManager::DoCoopJoinInfoUpdating() {
    for (int i = 0; i < 2; ++i) {
        if (coopJoin[i].port != -1) {
            unsigned int device = g_inputMgr.FindDeviceIDFromPortNum(coopJoin[i].port);
            coopJoin[i].mask = 1 << device;
            if (i == 0) {
                g_GameSettings.profile->profileIndex[0] = device;
                g_GameSettings.profile->profileIndex[1] = g_GameSettings.profile->profileIndex[1];
            } else if (i == 1) g_GameSettings.profile->profileIndex[1] = device;
        }
    }
}
#endif
