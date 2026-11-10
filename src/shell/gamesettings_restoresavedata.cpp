// CGameSettings: the settings singleton and its saved data.
#include "GameSettingsViews.h"

void CGameSettings::RestoreSaveData() { profile->saveData = levelStartData; }
