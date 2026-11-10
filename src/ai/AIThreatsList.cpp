// AI-assisted reconstruction from GR8E69. Fragment of the original AIThreats.cpp; the
// class, method and enum names come from the symbol table, member names are inferred.
#include <stdlib.h>
#include "AIBehavior.h"
#include "ScriptTimers.h"

void CAIThreatEvaluation::AddThreat(const CAIThreat &threat) {
    ScoreInitialThreat(InsertThreat(threat));
    Reevaluate();
}

void CAIThreatEvaluation::Reevaluate() {
    RankThreats();
    if (mostImportant && !mostImportant->reacted)
        ReactToThreat(mostImportant);
}

void CAIThreatEvaluation::AIDied(BSObject *object) {
    int notified = 0;
    for (int i = 0; i < 15; i++) {
        if (slots[i].used && slots[i].threat.source == object) {
            if (slots[i].reacted && !notified) {
                BSObjectTriggerEvent(scriptObject, 458, (void *)i, object, true);
                notified = 1;
            }
            ClearSlot(i);
        }
    }
    Reevaluate();
}

int CAIThreatEvaluation::HaveThreat() { return mostImportant != 0; }

CVector3 &CAIThreatEvaluation::GetPositionOfMostImportantThreat() { return mostImportant->threat.position; }

void CAIThreatEvaluation::Update(float dt) {
    for (int i = 0; i < 15; i++) {
        slots[i].score -= 0.01f;
        if (!(slots[i].score >= 0.0f))
            ClearSlot(i);
    }
    DebugDraw();
}

void CAIThreatEvaluation::FinishedInvestigation() {
    for (int i = 0; i < 15; i++) {
        if (slots[i].reacted)
            ClearSlot(i);
    }
    Reevaluate();
}

void CAIThreatEvaluation::AddCorpseThreat(BSObject *) {}

void CAIThreatEvaluation::OnStateChange(EAIHighLevelState previous, EAIHighLevelState next) {
    if (mostImportant) {
        if (next == 2 && previous != 3)
            ReactToThreat(mostImportant);
        else if (next == 3)
            ReactToThreat(mostImportant);
    }
}

int CAIThreatEvaluation::InsertThreat(const CAIThreat &threat) {
    for (int i = 0; i < 15; i++) {
        if (!slots[i].used) {
            InitializeSlot(i, threat);
            count++;
            return i;
        }
        if (slots[i].rank == 0 && count == 15) {
            InitializeSlot(i, threat);
            return i;
        }
    }
    return 0;
}

void CAIThreatEvaluation::InitializeSlot(int slot, const CAIThreat &threat) {
    slots[slot].used = 1;
    slots[slot].threat = threat;
    slots[slot].rank = 0;
    slots[slot].initialTime = 0.0f;
    slots[slot].reacted = 0;
    slots[slot].score = 0.0f;
}

static int CompareThreats(const void *a, const void *b) {
    float difference = ((const CAIThreatEvaluation::ThreatInfo *)a)->score - ((const CAIThreatEvaluation::ThreatInfo *)b)->score;
    if (difference < 0.0f)
        return -1;
    return difference > 0.0f;
}

void CAIThreatEvaluation::RankThreats() {
    qsort(slots, 15, sizeof(ThreatInfo), CompareThreats);
    int rank = 0;
    ThreatInfo *info = slots;
    for (int i = 0; i < 15; i++) {
        if (slots[i].used) {
            mostImportant = info;
            slots[i].rank = rank;
            rank++;
        }
        info++;
    }
}

float GetScoreStartByType(CAIThreat::ThreatType type) {
    float score = 0.0f;
    switch ((int)type) {
    case 7: return 1.0f;
    case 5: return 2.0f;
    case 4: return 3.0f;
    case 1: return 4.0f;
    case 3: return 5.0f;
    case 6: return 6.0f;
    case 2: break;
    }
    return score;
}

