// A fragment of the original SearchNode.cpp: A* search nodes drawn from a fixed pool. Class, method and
// static names come from the symbol table; member names are inferred from this code.
#include "AISplinePath.h"
#include "AIPathSearch.h"
#include "CLine2.h"
void DebugMsg(const char *, ...);

static inline CVector2 Flat(float x, float y) {
    CVector2 v;
    v.x = x;
    v.y = y;
    return v;
}

// Links a pool's nodes into a free list; its body is an out-of-line instance elsewhere.
class CAreaSearchNodePoolInit {
public:
    CAreaSearchNodePoolInit(CAreaSearchNode *pool, int count) : pool(pool), count(count) {}
    void Init();
    CAreaSearchNode *pool;
    int count;
};

extern CAreaSearchNode s_rsnAreaAStarSearchNodePool[];
extern CAreaSearchNodePoolInit s_AStarSearchNodePoolInit;
extern CAreaSearchNode *s_rsnAreaAStarSearchNodeFreeList;

int CAreaSearchNode::DirectPathToSearchNodeOk(CAreaSearchNode *target) {
    CLine2 line = CLine2(Flat(position.x, position.y), Flat(target->position.x, target->position.y));
    int result;
    if (!line.IsDegenerate())
        result = DirectPathOk(line, target);
    else
        result = 1;
    return result;
}

int CAreaSearchNode::DirectPathToPositionOk(const CVector3 &end, CAreaSearchNode *target) {
    CLine2 line = CLine2(Flat(position.x, position.y), Flat(end.x, end.y));
    if (line.IsDegenerate())
        return 1;
    if (!DirectPathOk(line, target))
        return 0;
    CAreaSearchNode *last = this;
    while (last->next != target)
        last = (CAreaSearchNode *)last->next;
    CAreaPathNode *hit = target->pathNode->LineIntersectsLinkEdges(line, 0, 0);
    if (!hit || hit != last->pathNode)
        return 0;
    return 1;
}

