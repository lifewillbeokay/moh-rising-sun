// AI-assisted reconstruction from GR8E69. Fragments of the original AIAreaPathFinding.cpp;
// method names come from the symbol table, member names are inferred.
#include "AIBehavior.h"

void CAIAreaPathFinding::GetClosestPositionOnAStar(CVector3 *position) const { *position = closestPosition; }

void CAIAreaPathFinding::EnterAStarVolume(int volume) {
    currentVolume = volume;
    volumeCount++;
}

void CAIAreaPathFinding::ExitAStarVolume(int) {
    if (--volumeCount == 0)
        currentVolume = volumeCount;
}
