// HandlerSaveSlotSelect and HandlerLoadSlotSelect, the save and load slot screens; see docs/Frontend.md.
#include "PackageHandlers.h"
#include "GameSettings.h"
#include "GameSettingsViews.h"
#include "PauseMenu.h"
#include <string.h>
#include <stdio.h>
void HandlerSaveSlotSelect::OnCancel() { Pop(); }
void HandlerSaveSlotSelect::OnPop() { g_PauseScreen.inMenu = 0; }
HandlerLoadSlotSelect::HandlerLoadSlotSelect() {}
HandlerLoadSlotSelect::~HandlerLoadSlotSelect() {}
