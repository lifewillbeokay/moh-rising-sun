// CGameSettings: the settings singleton and its saved data.
#include "GameSettingsViews.h"

int CGameSettings::SerializingIn() const { return serializeMode == 2; }
int CGameSettings::SerializingOut() const { return serializeMode == 1; }
static int findSavePointSlot(int id) {
    int i;
    for (i = 0; i < 32; i++) if (g_uSavePointID[i] == id) return i;
    for (i = 0; i < 32; i++) if (g_uSavePointID[i] == 0) return i;
    return -1;
}
void CGameSettings::RegisterSavePoint(int id, bool soft) {
    int slot = findSavePointSlot(id);
    g_bSavePointSoft[slot] = soft;
    g_uSavePointID[slot] = id;
}
void CGameSettings::TriggerSavePointResponse(int id, bool soft) {
    if (APlayerStartExistsForSavePoint(id)) {
        savePointPending = 1;
        savePointSoft = soft;
        profile->saveData.savePointID = (unsigned char)id;
    }
}
