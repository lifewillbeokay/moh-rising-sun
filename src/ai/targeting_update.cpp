// AI-assisted reconstruction from GR8E69; see docs/Targeting.md.
#include "AITargeting.h"
void CAITargeting::Update() {
    if (UpdateReactionTime()) {
        if (GetTarget().aiObject)
            UpdateTargetPosition();
        else if (GetTarget().scriptObject)
            UpdateNonAITargetPosition();
    }
}
