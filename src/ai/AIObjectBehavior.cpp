// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Fragments of the original AIObject.cpp; member names are descriptive.
#include "AITargeting.h"

int CAIObject::ChooseBehavior() {
    CBehavior *best = FindBestBehavior();
    if (best) {
        nextBehavior = best;
        return 1;
    } else {
        previousBehavior = 0;
        nextBehavior = 0;
    }
    return 0;
}

int CAIObject::ChooseNewBehavior() {
    CBehavior *best = FindBestBehavior();
    int chosen;
    if (best == currentBehavior) {
        previousBehavior = 0;
        nextBehavior = 0;
        chosen = 0;
    } else {
        nextBehavior = best;
        chosen = 1;
    }
    return chosen;
}

int CAIObject::StartBehavior() {
    int started = nextBehavior->StartBehavior();
    if (started)
        currentBehavior = nextBehavior;
    nextBehavior = 0;
    return started;
}

void CAIObject::StopBehavior(bool immediate) {
    if (currentBehavior)
        currentBehavior->StopBehavior(immediate);
}

void CAIObject::ClearBehavior() {
    currentBehavior->ReleaseResources();
    previousBehavior = currentBehavior;
    currentBehavior = 0;
}

int CAIObject::GetBehaviorCapability(ECapability capability) {
    int result;
    if (!currentBehavior)
        result = 0;
    else
        result = currentBehavior->GetCapabilities(capability);
    return result;
}
