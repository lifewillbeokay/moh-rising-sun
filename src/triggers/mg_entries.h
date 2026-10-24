// AI-assisted scoped reconstruction from GR8E69; see docs/TriggerObject.md.
#ifndef GAME_MG_TRIGGER_ENTRIES_H
#define GAME_MG_TRIGGER_ENTRIES_H
#include "TriggerObject.h"
// Descriptive twelve-byte entry view established by the original producer/consumer.
struct MGTriggerEntryView {
    TriggerObject *sourceTrigger;
    TriggerObject *positionTrigger;
    unsigned int unknown08;
};
extern MGTriggerEntryView g_pMGTriggerObject[32];
#endif
