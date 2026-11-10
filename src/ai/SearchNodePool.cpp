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

float CAreaSearchNode::GetHeuristicValue(const CVector3 &position) {
    CVector3 closest;
    return pathNode->GetClosestPosition(position, closest);
}

float CAreaSearchNode::GetCostValue(const CAreaSearchNode *other) {
    CVector3 offset;
    offset = other->position;
    return offset.Distance(position);
}

void ResetSearchNodePool() {
    CAreaSearchNodePoolInit &init = s_AStarSearchNodePoolInit;
    CAreaSearchNode *list = 0;
    for (int i = 0; i < init.count; i++) {
        init.pool[i].freeLink = list;
        init.pool[i].pathNode = 0;
        init.pool[i].open = 0;
        list = &init.pool[i];
    }
    s_rsnAreaAStarSearchNodeFreeList = list;
}

CAreaSearchNode *CAreaSearchNode::GetFreeNode() {
    CAreaSearchNode *node = s_rsnAreaAStarSearchNodeFreeList;
    CAreaSearchNode *result;
    if (!node) {
        DebugMsg("A* Pathfinding: Out of search nodes.");
        result = 0;
    } else {
        s_rsnAreaAStarSearchNodeFreeList = node->freeLink;
        result = new (node) CAreaSearchNode;
    }
    return result;
}

void CAreaSearchNode::DeleteNode(CAreaSearchNode *node) {
    node->~CAreaSearchNode();
    node->freeLink = s_rsnAreaAStarSearchNodeFreeList;
    s_rsnAreaAStarSearchNodeFreeList = node;
}

void CAreaSearchNode::Reset() {
    CAreaSearchNodePoolInit init(s_rsnAreaAStarSearchNodePool, 1228);
    init.Init();
}

CAreaSearchNode *CAreaSearchNode::GetNextNonOptimizedNode() {
    for (CAreaSearchNode *node = (CAreaSearchNode *)GetNextElement(); node; node = (CAreaSearchNode *)node->GetNextElement()) {
        if (!node->optimized)
            return node;
    }
    return 0;
}

