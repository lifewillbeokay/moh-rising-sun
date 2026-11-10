// HandlerLeaderboard, the multiplayer leaderboard: number fields; see docs/Frontend.md.
#include "PackageHandlers.h"
#include "GameSettings.h"
#include "GameSettingsViews.h"
#include "PauseMenu.h"
#include <string.h>
#include <stdio.h>
void HandlerLeaderboard::fillNumString(const char *name, int value, bool total) {
    char buffer[64];
    if (total) sprintf(buffer, "/%d", value);
    else sprintf(buffer, "%3d", value);
    SetObjectString(package, name, buffer);
}
void HandlerLeaderboard::fillNumGroupString(FEGroup *group, const char *name, int value, bool total) {
    char buffer[64];
    if (total) sprintf(buffer, "/%d", value);
    else sprintf(buffer, "%3d", value);
    SetGroupString(group, name, buffer);
}
