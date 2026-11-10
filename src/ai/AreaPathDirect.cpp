// AI-assisted reconstruction from GR8E69. Fragments of the original AreaPathNode.cpp;
// function and variable names come from the symbol table, member names are inferred.
// See docs/Behaviors.md.
#include "AISplinePath.h"

bool DirectPath(CAreaPathNode *node, CLine2 &line) {
    if (!node)
        return false;
    int previous = -1;
    CVector3 end(line.end.x, line.end.y, 0.0f);
    for (;;) {
        if (node->ContainsPoint(end))
            return true;
        int next = node->GetNextNode(line, previous);
        if (next == -1)
            return false;
        previous = AIPathFinding::GetIndexByPathNode(node);
        node = AIPathFinding::GetPathNodeByIndex(next);
    }
}

bool ClipLineToBorderEdge(CAreaPathNode *node, CVector3 &from, CVector3 &to, CAreaPathNode **outNode) {
    if (node) {
        CVector2 a, b, hit;
        a.Set(from.x, from.y);
        b.Set(to.x, to.y);
        CLine2 line(a, b);
        if (a.x == b.x && a.y == b.y)
            return false;
        CAreaPathNode *previous = 0;
        int tries = 49;
        do {
            if (node->ContainsPoint(to)) {
                *outNode = node;
                return false;
            }
            CAreaPathNode *next = node->LineIntersectsLinkEdges(line, &hit, previous);
            if (!next) {
                if (node->LineIntersectsBorderEdges(line, &hit)) {
                    to.x = hit.x;
                    to.y = hit.y;
                } else {
                    to.x = from.x;
                    to.y = from.y;
                }
                *outNode = node;
                return true;
            }
            previous = node;
            node = next;
        } while (--tries);
    }
    return false;
}
