// HandlerLoadSlotSelect, the load slot screen: cancel; see docs/Frontend.md.
#include "PackageHandlers.h"
#include "GameSettings.h"
#include "GameSettingsViews.h"
#include "PauseMenu.h"
#include <string.h>
#include <stdio.h>
void HandlerLoadSlotSelect::OnCancel() { Pop(); }
