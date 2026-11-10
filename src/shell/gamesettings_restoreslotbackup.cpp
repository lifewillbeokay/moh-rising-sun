// CGameSettings: the settings singleton and its saved data.
#include "GameSettingsViews.h"

void CGameSettings::RestoreSlotBackup() {
    if (slotBackupValid) profile->profiles[profile->profileIndex[slotBackupPlayer]].saves[slotBackupSlot] = slotBackup;
}
void CGameSettings::RememberProfileDevices() {
    for (int i = 0; i < 4; i++) profileDevice[i] = profile->profiles[i].device;
}
void CGameSettings::RestoreProfileDevices() {
    for (int i = 0; i < 4; i++) profile->profiles[i].device = profileDevice[i];
}
