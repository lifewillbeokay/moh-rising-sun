// PackageHandlerMain, the first menu; see docs/Frontend.md.
#include "PackageHandlers.h"
#include "GameSettings.h"
#include "MohLoader.h"
#include "Shell.h"
void PackageHandlerMain::HandleTick() {
    HandlerProfileSelect::InSelection();
    HandlerMPProfile::InSelection();
}
int PackageHandlerMain::HandleEvent(unsigned int event, FEObject *object, unsigned long) {
    HandleSaveGameEvent(event, object);
    if (event == FEHashUpper("Setup First Menu")) {
        g_Shell.inCoop = 0;
        g_Shell.inMultiplayer = 0;
        g_Shell.unknown_18c0 = 0;
        g_GameSettings.profile->wasLastGameSinglePlayer = 1;
        g_Shell.unknown_3bc8 = 0;
        g_inputMgr.controllerChecksEnabled = 1;
        package->controlMask = 255;
        g_inputMgr.CheckControllers(false);
        SetScript(package, "Online Multiplayer Text", "Hide");
        DisableButton(package, "Online Multiplayer Text");
    } else if (event == FEHashUpper("Query Game Mode")) {
        SCoreGameSettings *core = g_GameSettings.profile;
        if (core->field_32074 & 0x40000000) {
            if (core->saveData.quitFlag) {
                engine->QueueMessage(FEHashUpper("Launch SingleMulti"), 0, package, (FEObject *)-3, 0);
            } else {
                g_Shell.inMultiplayer = 1;
                engine->QueueMessage(FEHashUpper("Launch Multiplayer Game Setup"), 0, package, (FEObject *)-3, 0);
            }
        } else if (core->profiles[core->profileIndex[0]].saveNamed) {
            if (g_GameFlow.levelIsShell && !g_GameSettings.WasLastGameSinglePlayer()) {
                engine->QueueMessage(FEHashUpper("Launch Coop Main"), 0, package, (FEObject *)-3, 0);
            } else {
                engine->QueueMessage(FEHashUpper("Launch Single Player Main"), 0, package, (FEObject *)-3, 0);
            }
        } else {
            if (MohLoader::Instance()->gameDataValid != 1) {
                engine->QueueMessage(FEHashUpper("Launch SingleMulti"), 0, package, (FEObject *)-3, 0);
            } else {
                engine->QueueMessage(FEHashUpper("Launch Single Player Main"), 0, package, (FEObject *)-3, 0);
            }
            FEWideString *name = g_Shell.localizer->GetString("DEFAULT_PROFILE_NAME", 0);
            if (name && name->Length() <= 63) {
                char buffer[64];
                name->Convert(buffer);
                g_GameSettings.CopyNameIntoSettings(buffer, 0);
            }
        }
    } else if (event == FEHashUpper("Start Online Multiplayer")) {
        g_Shell.unknown_18c0 = 1;
    } else if (event == FEHashUpper("Reset Timer")) {
        engine->QueueMessage(FEHashUpper("Reset Timer"), 0, package, (FEObject *)-3, 0);
    }
    HandleMovieEvent(event, object);
    HandleFEngQueries(event, object);
    return 0;
}
