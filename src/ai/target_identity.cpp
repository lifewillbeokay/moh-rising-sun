// AI-assisted reconstruction from GR8E69; see docs/Targeting.md.
#include "AITargeting.h"
bool CAITarget::Matches(const CAITarget &other) {
    if (nonAI == other.nonAI) {
        if (nonAI && scriptObject == other.scriptObject)
            return true;
        if (aiObject == other.aiObject)
            return true;
    }
    return false;
}
BSObject *CAITarget::GetScriptObject() const {
    if (nonAI)
        return scriptObject;
    if (aiObject) {
        return aiObject->scriptObject;
    } else {
        return 0;
    }
}

void CAITarget::SetAsNonAITarget(BSObject *object) {
    scriptObject = object;
    aiObject = 0;
    nonAI = 1;
}
void CAITarget::Nullify() {
    aiObject = 0;
    scriptObject = 0;
    nonAI = 0;
}
void CAITarget::SetAsAITarget(CAIObject *object) {
    aiObject = object;
    scriptObject = 0;
    nonAI = 0;
}
CAITarget::CAITarget(CAIObject *object) {
    aiObject = object;
    scriptObject = 0;
    nonAI = 0;
}
CAITarget::CAITarget() {
    aiObject = 0;
    scriptObject = 0;
    nonAI = 0;
}
