// AI-assisted reconstruction from GR8E69. A fragment of the original AIAreaPathFinding.cpp;
// method names come from the symbol table, member names are inferred. See docs/Behaviors.md.
#include "AIPathSearch.h"

void CAIAreaPathFinding::TrimSolutionNodeQueue(CAreaSearchNode *solution, CAreaSearchNodeList &list) {
    CAreaSearchNode *node;
    for (node = solution; node; node = node->freeLink) {
        node->pathNode->searchNode = 0;
        node->open ^= 1;
    }
    for (node = (CAreaSearchNode *)list.head; node;) {
        CAreaSearchNode *next = (CAreaSearchNode *)node->GetNextElement();
        list.RemoveElement(node);
        if (node->open) {
            node->open ^= 1;
            CAreaSearchNode::DeleteNode(node);
        }
        node = next;
    }
    for (node = solution; node; node = node->freeLink)
        list.AddElementAtBeginning(node);
}

void CAIAreaPathFinding::CleanupAStarPathWalk() {
    CAreaSearchNode *node;
    while ((node = (CAreaSearchNode *)solutionPath.head) != 0) {
        solutionPath.RemoveElement(node);
        CAreaSearchNode::DeleteNode(node);
    }
}
