// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Fragments of the original AIObject.cpp; member names are descriptive.
#include "AITargeting.h"

int CAIObject::GetPreviousBehaviorType() {
    int result;
    if (!previousBehavior)
        result = 0;
    else
        result = previousBehavior->GetType();
    return result;
}

int CAIObject::GetNextBehaviorType() {
    int result;
    if (!nextBehavior)
        result = 0;
    else
        result = nextBehavior->GetType();
    return result;
}

BSObject *CAIObject::GetNextBehaviorScript() {
    BSObject *result;
    if (!nextBehavior)
        result = 0;
    else
        result = nextBehavior->GetScriptObject();
    return result;
}
