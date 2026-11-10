// CGameSettings: statistics and level records.
#include "GameSettingsViews.h"
#include "Objectives.h"
#include "Toilet.h"
void iDebugMsg(const char *, ...);
void DebugMsg(const char *, ...);
extern int g_bMPEndLevel;

void CGameSettings::LoadData() {
    loadedData = MohLoader::Instance()->gameDataValid;
}
