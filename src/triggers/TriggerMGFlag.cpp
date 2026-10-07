// AI-assisted reconstruction from GR8E69; see docs/TriggerObject.md.
#include "TriggerObject.h"
void MarkMGAsUsed(TriggerObject *trigger, bool used) {
    if (used)
        trigger->flags.mgUsed = 1;
    else
        trigger->flags.mgUsed = 0;
}
int IsMGUsed(TriggerObject *trigger) { return trigger->flags.mgUsed; }
