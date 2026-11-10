// AI-assisted reconstruction from GR8E69. Fragment of the original AIThreats.cpp; the
// class, method and enum names come from the symbol table, member names are inferred.
#include <stdlib.h>
#include "AIBehavior.h"
#include "ScriptTimers.h"

int GetReactionByType(CAIThreat::ThreatType type, EAIHighLevelState state) {
    int reaction = 0;
    switch ((int)type) {
    case 2:
        reaction = state == 2;
        break;
    case 4:
    case 5:
    case 7:
        reaction = 0;
        break;
    case 1:
    case 3:
    case 6:
        reaction = 1;
        break;
    }
    return reaction;
}

void CAIThreatEvaluation::ReactToThreat(ThreatInfo *info) {
    EAIHighLevelState state = aiObject->highLevelState;
    if (state == 2 || state == 3) {
        unsigned short event = GetReactionByType(info->threat.type, state) == 0 ? 456 : 457;
        BSObjectTriggerEvent(scriptObject, event, &info->threat, info->threat.source, true);
        info->reacted = 1;
    }
}
