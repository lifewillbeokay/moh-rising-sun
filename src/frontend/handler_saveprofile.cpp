// HandlerSaveProfile's constructor and destructor; see docs/Frontend.md.
#include "PackageHandlers.h"
#include "GameSettings.h"
#include "MemoryCardDevice.h"
#include "PauseMenu.h"
HandlerSaveProfile::HandlerSaveProfile() {
    unknown_54 = 1;
    unknown_50 = 1;
    g_GameSettings.RememberProfileDevices();
    g_hMemoryCard->TellTheGamecubeToShutUp(true);
}
HandlerSaveProfile::~HandlerSaveProfile() {
    if (g_PauseScreen.inMenu) {
        g_GameSettings.RestoreProfileDevices();
    }
    g_hMemoryCard->TellTheGamecubeToShutUp(false);
}
