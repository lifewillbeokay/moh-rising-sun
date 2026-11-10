// AI-assisted reconstruction from GR8E69. Fragments of the original AIAreaPathFinding.cpp;
// method names come from the symbol table, member names are inferred.
#include "AIBehavior.h"

void CAIAreaPathFinding::FinishPath() { pathFinished = 1; }
int CAIAreaPathFinding::GetWalkType() { return 2; }
void CAIAreaPathFinding::StopWalk() { StopAStarPathWalk(); }
