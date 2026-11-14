// AI-assisted GR8E69 reconstruction; see docs/research/frontend-screens.md.
#ifdef DECOMP_COMPARE
#include "PackageHandlers.h"
#include "Shell.h"
#include "GameSettings.h"
extern CMemoryCard *g_hMemoryCard;
void HandlerCoop4WaySplit::GotoNextScreen() {
    engine->QueuePackageSwitch("CoopTwoPlayerCampaign.fng", package);
}
int HandlerCoop4WaySplit::Init(const char *) {
    g_Shell.inCoop = 1;
    shell->unknown_18a2[0] &= 0xf0;
    shell->unknown_18a2[1] &= 0xf0;
    shell->coopJoinBits &= 0xf0;
    shell->mpPlayerCount &= 0xf0;
    g_GameSettings.profile->unknown_3201c[0] = 2;
    shell->coopJoinBits |= 3;
    g_inputMgr.controllerChecksEnabled = 1;
    g_Shell.unknown_3bc8 = 0;
    playerCount = g_GameSettings.profile->unknown_3201c[0];
    HandlerMPProfile::m_lastProfile = 0;
    engine->QueuePackagePush("MPJoin.fng", package, 0xaf);
    engine->QueuePackagePush("MPJoin.fng", package, 0xb0);
    g_Shell.ResetCoopJoinInfo(-1);
    shell->quadrantUsed[0] = 0; shell->quadrantUsed[1] = 0;
    g_hMemoryCard->OnEnterInteractiveMode(); return 0;
}
#endif
