// AI-assisted reconstruction from GR8E69; see docs/research/frontend-popup-list.md.
#include "PackageHandlers.h"
#include "Shell.h"
#include "GameSettings.h"
void CSaveGamePopup::Update(float delta) {
    if (!g_GameFlow.unknown_0 && !g_GameFlow.levelEnding) g_inputMgr.CheckControllers(true);
    engine->Update((long)(delta * 20.0f));
}
