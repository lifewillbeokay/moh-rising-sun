// AI-assisted reconstruction from GR8E69; see docs/TriggerObject.md.
#include "mg_entries.h"
void ClearMGTriggerObjects() {
    MGTriggerEntryView *entry = g_pMGTriggerObject;
    for (int i = 0; i < 32; ++i, ++entry) {
        entry->sourceTrigger = 0;
        entry->positionTrigger = 0;
        entry->unknown08 = 0;
    }
}
