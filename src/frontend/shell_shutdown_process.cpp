// ShellManager's Shutdown and Process; see docs/Frontend.md.
#include "ShellHandlerMap.h"
int ShellManager::Shutdown() {
    {
            ShellHandlerMap::iterator it = handlers.begin();
        ShellHandlerMap::iterator end = handlers.end();
        for (; it != end; ++it) {
            factory->DestroyPackageHandler((*it).second);
        }
    }
    handlers.clear();
    delete unplugged;
    delete engine;
    delete factory;
    delete localizer;
    delete loader;
    unplugged = 0;
    engine = 0;
    factory = 0;
    localizer = 0;
    loader = 0;
    return 0;
}
void ShellManager::Process(unsigned int ticks) {
    if (updateOnce) {
        engine->Update(1);
        updateOnce = 0;
    } else {
        if (inMultiplayer) {
            DoMPJoinScreenUpdate();
        } else if (inCoop) {
            DoCoopJoinScreenUpdate();
        }
        UpdateUnplugged();
        if (!unknown_3bc8) {
            if (inCoop) {
                DoCoopPackageControlMaskUpdating();
                DoCoopJoinInfoUpdating();
            }
            if (!unknown_3bc8 && (inMultiplayer || inCoop)) {
                DoPackageControlMaskUpdating();
            }
        }
        engine->Update(ticks);
    }
    factory->Process();
    loader->Process();
    if (g_GameFlow.levelIsShell && g_inputMgr.GetValue(0, 128)) {
        MoveShellCamera();
    }
}
