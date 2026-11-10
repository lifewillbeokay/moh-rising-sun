// AI-assisted reconstruction from GR8E69. A fragment of the original AIAreaPathFinding.cpp;
// method names come from the symbol table, member names are inferred. See docs/Behaviors.md.
#include "AIPathSearch.h"
#include "ScriptTimers.h"

void CAIAreaPathFinding::WalkAStarPath() {
    UpdateAStarPathWalk();
    float distance;
    if ((int)locomotion->movePointType == 4)
        distance = physics->position.Distance(destination);
    else
        distance = physics->position.DistanceXY(destination);
    if (!(distance - locomotion->arrivalDistance >= 0.5f) && walkPhase == 1) {
        if (fieldA4) {
            walkPhase = 2;
            fieldA4 = 0;
            UpdateAStarPathWalk();
        } else {
            pathFinished = 1;
            BSObjectTriggerEvent(scriptObject, 154, 0, 0, true);
        }
    }
}

void CAIAreaPathFinding::ClearSearchNodeQueue(CAreaSearchNodeQueue &queue) { queue.Clear(); }

void CAIAreaPathFinding::ClearSearchNodeList(CAreaSearchNodeList &list) {
    while (!list.IsEmpty()) {
        CAreaSearchNode *node = (CAreaSearchNode *)list.head;
        list.RemoveElement(node);
        CAreaSearchNode::DeleteNode(node);
    }
}
