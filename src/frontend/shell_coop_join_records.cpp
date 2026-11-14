// AI-assisted GR8E69 reconstruction; see docs/research/frontend-screens.md.
#ifdef DECOMP_COMPARE
#include "ShellManager.h"
#include "Shell.h"
#include "GameSettings.h"
void ShellManager::ResetCoopJoinInfoFromPortNum(unsigned char port) {
    for (unsigned char i = 0; i < 2; ++i) {
        if (coopJoin[i].port == port) {
            coopJoin[i].package = 0; coopJoin[i].port = -1; coopJoin[i].mask = 0;
            return;
        }
    }
}
void ShellManager::SetCoopJoinInfo(FEPackage *package, unsigned long mask) {
    int i = 0;
    while (coopJoin[i].package && ++i < 2) {}
    coopJoin[i].package = package;
    unsigned char device = GetDeviceIndexFromControlMask(package->controlMask);
    coopJoin[i].port = g_inputMgr.GetPortNum(device);
    if (i == 0) {
        g_GameSettings.profile->profileIndex[0] = device;
        g_GameSettings.profile->profileIndex[1] = g_GameSettings.profile->profileIndex[1];
    } else if (i == 1) g_GameSettings.profile->profileIndex[1] = device;
    coopJoin[i].mask = mask;
}
#endif
