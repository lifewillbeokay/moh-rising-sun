// HandlerSaveSlotBase, the save slot screens: construction and navigation; see docs/Frontend.md.
#include "PackageHandlers.h"
#include "GameSettings.h"
#include "GameSettingsViews.h"
#include "PauseMenu.h"
#include <string.h>
#include <stdio.h>
void HandlerSaveSlotBase::Push(const char *name) { g_Shell.Push(name); }
void HandlerSaveSlotBase::Swap(const char *name, FEPackage *package) { g_Shell.Swap(name, package); }
HandlerSaveSlotBase::HandlerSaveSlotBase() {}
HandlerSaveSlotBase::~HandlerSaveSlotBase() {}
