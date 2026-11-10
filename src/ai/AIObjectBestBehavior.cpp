// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Fragments of the original AIObject.cpp; member names are descriptive.
#include "AITargeting.h"

float CAIObject::GetHearingDistance() { return hearingRadius; }

float CAIObject::GetAIValueByName(const char *) { return 0.0f; }

CBehavior *CAIObject::FindBestBehavior() {
    int count;
    CBehavior **list;
    switch (highLevelState) {
    case 1: count = behaviorLists[0].count; list = behaviorLists[0].behaviors; break;
    case 2: count = behaviorLists[1].count; list = behaviorLists[1].behaviors; break;
    case 3: count = behaviorLists[2].count; list = behaviorLists[2].behaviors; break;
    case 4: count = behaviorLists[3].count; list = behaviorLists[3].behaviors; break;
    default: return 0;
    }
    float bestValue = 0.0f;
    CBehavior *best = 0;
    for (int i = 0; i < count; i++) {
        if (list[i]->enabled) {
            float value = list[i]->GetCurrentHeuristicScore();
            if (!(value <= bestValue)) {
                best = list[i];
                bestValue = value;
            }
        }
    }
    if (bestValue > 0.01f)
        return best;
    return 0;
}
