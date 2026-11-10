// CGameSettings: the settings singleton and its saved data.
#include "GameSettingsViews.h"

void CGameSettings::SaveData() { MohLoader::Instance()->ValidateGameData(); }
