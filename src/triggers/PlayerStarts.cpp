// AI-assisted reconstruction from GR8E69; see docs/TriggerObject.md.
#include "TriggerObject.h"
extern unsigned int g_pNumTriggers;
extern TriggerObject g_pTriggerObjects[];
extern int g_numPlayerStarts;
extern int g_playerStartIndices[20];
extern void DebugMsg(const char *, ...);

void FindPlayerStarts() {
    if (g_numPlayerStarts) return;
    TriggerObject *object = g_pTriggerObjects;
    for (unsigned int i = 0; i < g_pNumTriggers; ++i, ++object) {
        if (object->GetNativeBindingType() == 1 && object->GetChildType() == 0x271bd574) {
            g_playerStartIndices[g_numPlayerStarts++] = i;
            if (g_numPlayerStarts == 20) {
                DebugMsg("Player Starts Limit reached!\n");
                break;
            }
        }
    }
}
bool APlayerStartExistsForSavePoint(int savePoint) {
    TriggerObject *object = g_pTriggerObjects;
    for (unsigned int i = 0; i < g_pNumTriggers; ++i, ++object) {
        if (object->GetNativeBindingType() == 1 && object->GetChildType() == 0x271bd574 &&
            object->GetInt("SavePointID") == savePoint) return true;
    }
    return false;
}
