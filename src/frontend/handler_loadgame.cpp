// HandlerLoadGame, the load game overlay; see docs/Frontend.md.
#include "PackageHandlers.h"
#include "GameSettings.h"
#include "GameSettingsViews.h"
#include "PauseMenu.h"
#include "Shell.h"
enum GcSwapDiscsMode { GcSwapDiscsMode0 = 0 };
int LevelExistsOnCurrentDisc(int, int);
int SwapDiscs(GcSwapDiscsMode);
HandlerLoadGame::HandlerLoadGame() {}
HandlerLoadGame::~HandlerLoadGame() {}
int HandlerLoadGame::HandleEvent(unsigned int event, FEObject *object, unsigned long param) {
    if (event == FEHashUpper("Init Profile Name")) {
        SetProfileNameOnScreen(g_Shell.coopPausePlayer);
        HandlerCoopGameOptions::mSaveSlot = -1;
    } else {
        HandlerSaveSlotBase::HandleEvent(event, object, param);
    }
    return 0;
}
