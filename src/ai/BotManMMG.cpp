// AI-assisted reconstruction from GR8E69. A fragment of the original BotMan source: function
// names come from the symbol table, member names are inferred.
#include "AIBot.h"
#include "TriggerObject.h"

int MMG_Info::IsInUse() {
    if (trigger)
        return IsMGUsed(trigger);
    return 0;
}

void MMG_Info::SetInUse(bool used) {
    if (trigger)
        MarkMGAsUsed(trigger, used);
}
