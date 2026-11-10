// AI-assisted reconstruction from GR8E69. A fragment of the original AIAreaPathFinding.cpp;
// method names come from the symbol table, member names are inferred. See docs/Behaviors.md.
#include "AIPathSearch.h"

void CAIAreaPathFinding::Update() {
    CAreaPathNode *node = AIPathFinding::GetClosestPathNode(physics->position, -1);
    if (node && node->ContainsPoint(physics->position)) {
        positionValid = 1;
        onNetwork = 1;
        closestPosition = physics->position;
        currentNode = node;
    } else {
        onNetwork = 0;
    }
}
