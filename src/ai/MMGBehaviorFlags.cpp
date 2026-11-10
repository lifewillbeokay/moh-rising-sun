// AI-assisted reconstruction from GR8E69. Fragments of the original MountedMachineGunBehavior.cpp;
// method names come from the symbol table, member names are inferred.
#include "AIBehavior.h"
#include "AIFilterGlobal.h"
#include "ScriptTimers.h"

int CMountedMachineGunBehavior::GetCapabilities(ECapability capability) {
    switch ((int)capability) {
    case 1:
    case 5:
        return 1;
    case 2:
    case 3:
    case 6:
        if (arrived)
            return 1;
        return 0;
    case 7:
        return !arrived;
    default:
        return 0;
    }
}

void CMountedMachineGunBehavior::Arrived() { arrived = 1; }

void CMountedMachineGunBehavior::CantReachMMG() {
    cantReach = 1;
    heuristicScore = 0.0f;
    retryTime = g_aigAIFilterGlobalObject.time + 5.0f;
    BSObjectTriggerEvent(aiObject->scriptObject, 268, 0, 0, true);
}

void CMountedMachineGunBehavior::HitFromBehind() {
    hitFromBehind = 1;
    heuristicScore = 0.0f;
    retryTime = g_aigAIFilterGlobalObject.time + 5.0f;
    BSObjectTriggerEvent(aiObject->scriptObject, 268, 0, 0, true);
}

void CMountedMachineGunBehavior::ForceToMMG() { forced = 1; }
