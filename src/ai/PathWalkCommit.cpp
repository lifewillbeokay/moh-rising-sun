// AI-assisted reconstruction from GR8E69. A fragment of the original AIAreaPathFinding.cpp;
// method names come from the symbol table, member names are inferred. See docs/Behaviors.md.
#include "AIPathSearch.h"
#include "AITargeting.h"

extern CAIFilterGlobal g_aigAIFilterGlobalObject;

void CAIAreaPathFinding::SetupAStarPathWalkCommit() {
    commitTime = g_aigAIFilterGlobalObject.time + 1.0f;
    locomotion->updateTime = g_aigAIFilterGlobalObject.time + 0.3f;
}
