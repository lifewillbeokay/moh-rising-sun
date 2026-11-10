// HandlerSaveSlotSelect, the save slot screen; see docs/Frontend.md.
#include "PackageHandlers.h"
#include "GameSettings.h"
#include "GameSettingsViews.h"
#include "PauseMenu.h"
#include <string.h>
#include <stdio.h>
HandlerSaveSlotSelect::HandlerSaveSlotSelect() {}
HandlerSaveSlotSelect::~HandlerSaveSlotSelect() {}
void HandlerSaveSlotSelect::Push() { HandlerSaveSlotBase::Push("SaveSlotSelect.fng"); }
