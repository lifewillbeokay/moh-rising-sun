// AI-assisted reconstruction from GR8E69. Fragments of the original BuddyLeadBehavior.cpp;
// method names come from the symbol table, member names are inferred.
#include "TriggerObject.h"
#include "AIBehavior.h"
#include "ScriptTimers.h"
Checkpoint_t *GetNextCheckpoint(Checkpoint_t *);

int CBuddyLeadBehavior::StartBehavior() {
    if (!currentCheckpoint)
        currentCheckpoint = GetNextCheckpoint(0);
    BSObject *self = aiObject->filter->GetScriptObject();
    BSObject *checkpoint = currentCheckpoint->trigger->scriptObject;
    BSObjectTriggerEvent(checkpoint, 269, 0, self, true);
    BSObjectTriggerEvent(self, 336, 0, checkpoint, true);
    return 1;
}

void CBuddyLeadBehavior::StopBehavior(bool immediate) {
    BSObject *checkpoint = currentCheckpoint->trigger->scriptObject;
    if (immediate)
        BSObjectTriggerEvent(checkpoint, 271, 0, aiObject->filter->GetScriptObject(), true);
    else
        BSObjectTriggerEvent(checkpoint, 270, 0, aiObject->filter->GetScriptObject(), true);
}

int CBuddyLeadBehavior::GetCapabilities(ECapability capability) { return capability == Capability0; }
