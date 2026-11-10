// AI-assisted reconstruction from GR8E69. A fragment of the original AIAreaPathFinding.cpp;
// method names come from the symbol table, member names are inferred. See docs/Behaviors.md.
#include "AIPathSearch.h"

void CAIAreaPathFinding::TeleportToNextPositionOnPath() {
    CVector3 position;
    position = locomotion->GetCurrentObjectivePosition();
    locomotion->TeleportTo(position);
}

int CAIAreaPathFinding::GetNextWalkToPos(CVector3 &position) {
    if (currentPathNode) {
        CAreaSearchNode *next = (CAreaSearchNode *)currentPathNode->GetNextElement();
        if (next) {
            position = next->position;
            return 1;
        }
    }
    return 0;
}
