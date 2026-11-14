// AI-assisted GR8E69 reconstruction; see docs/research/frontend-screens.md.
#ifdef DECOMP_COMPARE
#include "PackageHandlers.h"
#include "Shell.h"
#include "GameSettings.h"
int PackageHandler::HandleCoopLevelStartEvent(unsigned int event, FEObject *) {
    int level = 0;
    if (event == FEHashUpper("Select Coop Level 01")) level = 1;
    else if (event == FEHashUpper("Select Coop Level 02")) level = 2;
    else if (event == FEHashUpper("Select Coop Level 03")) level = 3;
    else if (event == FEHashUpper("Select Coop Level 04")) level = 4;
    else if (event == FEHashUpper("Select Coop Level 05")) level = 5;
    else if (event == FEHashUpper("Select Coop Level 06")) level = 6;
    else if (event == FEHashUpper("Select Coop Level 07")) level = 7;
    else if (event == FEHashUpper("Select Coop Level 08")) level = 8;
    else if (event == FEHashUpper("Select Coop Level 09")) level = 9;
    else if (event == FEHashUpper("Select Coop Level 10")) level = 10;
    else if (event == FEHashUpper("Select Coop Level 11")) level = 11;
    if (level) {
        g_GameSettings.SetNextMissionAndLevel(5, level, 0);
        g_GameSettings.profile->field_32074 &= ~0x40000000;
        g_GameSettings.profile->field_32074 |= 0x08000000;
        g_GameSettings.profile->unknown_3201c[2] = 5;
        g_GameSettings.profile->unknown_3201c[0] = 2;
        g_GameFlow.startingMP = 1;
    }
    return 0;
}
#endif
