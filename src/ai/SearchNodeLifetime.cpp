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

CAreaSearchNode::CAreaSearchNode() {
    position.w = 1.0f;
    next = 0;
    pathNode = 0;
    optimized = 0;
    open = 0;
    unknown34 = 0;
}

CAreaSearchNode::~CAreaSearchNode() {
    if (pathNode->searchNode == this)
        pathNode->searchNode = 0;
    if (open)
        open ^= 1;
    next = 0;
    pathNode = 0;
}
