// AI-assisted reconstruction from GR8E69. Fragment of the original AIThreats.cpp; the
// class, method and enum names come from the symbol table, member names are inferred.
#include <stdlib.h>
#include "AIBehavior.h"
#include "ScriptTimers.h"

void CAIThreatEvaluation::ClearSlot(int slot) {
    if (slots[slot].used)
        count--;
    slots[slot].used = 0;
    slots[slot].score = 0.0f;
    slots[slot].reacted = 0;
    if (mostImportant == &slots[slot])
        mostImportant = 0;
}

void CAIThreatEvaluation::DebugDraw() {}
