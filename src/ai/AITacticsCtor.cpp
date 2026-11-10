// AI-assisted reconstruction of CAITactics, which scores positions around an AI object; see docs/Behaviors.md.
// Method names come from the symbol table; the score terms' names are inferred.
#include "AITargeting.h"
#include "BSGameObject.h"
#include "ScriptRuntime.h"
extern "C" float sqrtf(float);
CAITactics::CAITactics(CAIPhysics *physics, CAITargeting *targeting, CAIObject *owner)
    : physics(physics), targeting(targeting), owner(owner) {}
