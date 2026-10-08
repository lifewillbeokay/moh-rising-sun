// AI-assisted reconstruction from GR8E69; see docs/Targeting.md.
#include "AITargeting.h"
BSObject *CAITargeting::GetTargetScript() const {
    return target.GetScriptObject();
}
bool CAITargeting::IsNonAITarget() const {
    const CAITarget &current = target;
    if (current.nonAI) {
        if (current.scriptObject)
            return true;
    }
    return false;
}

bool CAITargeting::HasValidTarget() const {
    return this && GetTarget().HasTarget();
}
