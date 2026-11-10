// AI-assisted reconstruction from GR8E69. Fragments of the original BuddyLeadBehavior.cpp;
// method names come from the symbol table, member names are inferred.
#include "TriggerObject.h"
#include "AIBehavior.h"
Checkpoint_t *GetNextCheckpoint(Checkpoint_t *);

CBuddyLeadBehavior::CBuddyLeadBehavior() : checkpointDone(0), finished(0), heuristicScore(0.0f), currentCheckpoint(0) { enabled = 0; }

CBuddyLeadBehavior::~CBuddyLeadBehavior() {}
float CBuddyLeadBehavior::GetCurrentHeuristicScore() { return heuristicScore; }
int CBuddyLeadBehavior::GetHeuristicUpdateFrequency() { return 60; }

int CBuddyLeadBehavior::UpdateHeuristicScore() {
    if (checkpointDone && currentCheckpoint->reached) {
        currentCheckpoint = GetNextCheckpoint(currentCheckpoint);
        checkpointDone = 0;
    }
    float score;
    if (checkpointDone || finished || !(currentCheckpoint || GetNextCheckpoint(0)))
        score = 0.0f;
    else
        score = 100.0f;
    float previous = heuristicScore;
    heuristicScore = score;
    return previous != score;
}
