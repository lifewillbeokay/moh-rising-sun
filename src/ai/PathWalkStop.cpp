// AI-assisted reconstruction from GR8E69. A fragment of the original AIAreaPathFinding.cpp;
// method names come from the symbol table, member names are inferred. See docs/Behaviors.md.
#include "AIPathSearch.h"
#include "ScriptTimers.h"

void CAIAreaPathFinding::StopAStarPathWalk() {
    walkActive = 0;
    field80 = 0;
    currentPathNode = 0;
    locomotion->ClearWalkType();
    fieldAC = 0;
    fieldA8 = 0;
    fieldA4 = 0;
    locomotion->teleportPending = 0;
    CleanupAStarPathWalk();
}

void CAIAreaPathFinding::RemovePointersTo(CAIObject *object) {
    if (targetObject == object) {
        BSObjectTriggerEvent(scriptObject, 66, 0, 0, true);
        StopAStarPathWalk();
        targetObject = 0;
    }
}
