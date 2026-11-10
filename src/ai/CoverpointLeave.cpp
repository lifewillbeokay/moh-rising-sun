// AI-assisted reconstruction from GR8E69. Fragments of the original CoverpointBehavior.cpp;
// method names come from the symbol table, member names are inferred.
#include "SceneObjects.h"
#include "SceneNode.h"
#include "Weapon.h"
#include "AIBehavior.h"
#include "ScriptTimers.h"

void CCoverpointBehavior::LeaveCoverPoint() {
    if (coverPoint)
        coverPoint->FlagAsUnoccupied();
    coverPoint = 0;
    aiObject->locomotion.ClearWalkType();
    arrived = 0;
    soldier->StopOverrideLOSPosition();
    coverScript = 0;
    ended = 0;
}

