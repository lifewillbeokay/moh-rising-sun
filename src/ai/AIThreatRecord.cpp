// AI-assisted reconstruction from GR8E69. Fragment of the original AIThreats.cpp; the
// class, method and enum names come from the symbol table, member names are inferred.
#include <stdlib.h>
#include "AIBehavior.h"
#include "ScriptTimers.h"

CAIThreat::CAIThreat() {
    position.Zero();
    type = ThreatNone;
    source = 0;
    directed = false;
}

CAIThreat::CAIThreat(ThreatType threatType, const CVector3 &threatPosition, BSObject *threatSource, bool threatDirected) {
    type = threatType;
    position = threatPosition;
    source = threatSource;
    directed = threatDirected;
}

