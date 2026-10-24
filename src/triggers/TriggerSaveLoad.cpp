// AI-assisted reconstruction from GR8E69; see docs/TriggerObject.md.
#include "TriggerObject.h"
#include "ScriptTimers.h"
extern unsigned int g_pNumTriggers;
extern TriggerObject g_pTriggerObjects[];
void TriggerObject::Save() {
    TriggerObject *object = g_pTriggerObjects;
    for (unsigned int i = 0; i < g_pNumTriggers; ++i, ++object) {
        if (object->scriptObject)
            BSObjectTriggerEvent(object->scriptObject, 422, 0, 0, true);
    }
}

void TriggerObject::Load() {
    TriggerObject *object = g_pTriggerObjects;
    for (unsigned int i = 0; i < g_pNumTriggers; ++i, ++object) {
        if (object->scriptObject)
            BSObjectTriggerEvent(object->scriptObject, 422, 0, 0, true);
    }
}
