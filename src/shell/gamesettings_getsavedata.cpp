// CGameSettings: the settings singleton and its saved data.
#include "GameSettingsViews.h"

SSaveData *CGameSettings::GetSaveData(unsigned int slot, unsigned int player) const {
    return &profile->profiles[profile->profileIndex[player]].saves[slot];
}
void CGameSettings::SetCurrentCompartmentID(unsigned int player, int id) { profile->saveData.compartmentID[player] = id; }
int CGameSettings::GetCurrentCompartmentID(unsigned int player) const { return profile->saveData.compartmentID[player]; }
int CGameSettings::GetCurrentOneShotBankID() { return profile->saveData.oneShotBank; }
int CGameSettings::GetCurrentSavePointID() const { return profile->saveData.savePointID; }
