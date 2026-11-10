// AI-assisted reconstruction of CAITargeting::SetTarget; see docs/Behaviors.md.
#include "AITargeting.h"

// Each AI object counts the objects targeting it, and exposes the count to scripts capped at 15.
static inline void AddTargeter(CAIObject *object, int delta) {
    int count = object->targeterCount + delta;
    object->targeterCount = count;
    if (count <= 15)
        object->status.SetVarValue((aistatus_var)6, count);
    else
        object->status.SetVarValue((aistatus_var)6, 15);
}

CAIObject *CAITargeting::SetTarget(CAIObject *object) {
    CAIObject *previous = target.aiObject;
    CAITarget *current = &target;
    if (object == previous) {
        if (!previous) {
            current->Nullify();
            lastSeenSet = 0;
        }
        return target.aiObject;
    }
    visionVisible = 0;
    target.aiObject = object;
    if (object) {
        current->SetAsAITarget(object);
        lastSeenPosition = object->position;
        visionVisible = 1;
        lastSeenSet = 1;
        CalculateTargetPosition();
    } else {
        current->Nullify();
        lastSeenSet = 0;
    }
    if (object)
        AddTargeter(object, 1);
    if (previous)
        AddTargeter(previous, -1);
    return previous;
}
