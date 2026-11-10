// AI-assisted reconstruction from GR8E69. Fragments of the original MountedMachineGunBehavior.cpp;
// method names come from the symbol table, member names are inferred.
#include "AIBehavior.h"
#include "ScriptTimers.h"
#include "TriggerObject.h"
void BidOnMMG(TriggerObject *, CAIObject *, float);

CMountedMachineGunBehavior::CMountedMachineGunBehavior()
    : behaviorStyle((EAIHighLevelState)4), heuristicScore(0.0f), gunScript(0), bidTrigger(0), arrived(0), stopping(0), forced(0),
      cantReach(0), retryTime(0.0f), field3c(0.0f), hitFromBehind(0), field44(0) {}

CMountedMachineGunBehavior::~CMountedMachineGunBehavior() {
    if (bidTrigger) {
        BidOnMMG(bidTrigger, aiObject, 0.0f);
        bidTrigger = 0;
    }
}

float CMountedMachineGunBehavior::GetCurrentHeuristicScore() { return heuristicScore; }
int CMountedMachineGunBehavior::GetHeuristicUpdateFrequency() { return gunScript ? 1 : 60; }
