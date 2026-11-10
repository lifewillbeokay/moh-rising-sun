// AI-assisted reconstruction from GR8E69. Views of the A* search and area path node
// classes; class and method names come from the symbol table, member names are inferred.
// See docs/Behaviors.md.
#ifndef GAME_AI_PATH_SEARCH_H
#define GAME_AI_PATH_SEARCH_H
#include "AITargeting.h"
#include "LinkedList.h"
class CAreaPathNode;
class CLine2;
class CAreaSearchNode : public CLinkedListElement {
  public:
    CAreaSearchNode();
    ~CAreaSearchNode();
    void *operator new(unsigned int, void *p) { return p; }
    static CAreaSearchNode *GetFreeNode();
    static void DeleteNode(CAreaSearchNode *);
    static void Reset();
    float GetHeuristicValue(const CVector3 &);
    float GetCostValue(const CAreaSearchNode *);
    CAreaSearchNode *GetNextNonOptimizedNode();
    int DirectPathOk(const CLine2 &, CAreaSearchNode *);
    int DirectPathToSearchNodeOk(CAreaSearchNode *);
    int DirectPathToPositionOk(const CVector3 &, CAreaSearchNode *);
    void WalkDirect(CAreaSearchNode *, const CVector3 &);

    CAreaSearchNode *freeLink; // +0x04
    CAreaPathNode *pathNode;   // +0x08
    unsigned char unknown0c[4];
    CVector3 position;         // +0x10
    unsigned char unknown20[4];
    float bestCost;            // +0x24; compared by GetBestPath.
    unsigned char unknown28[4];
    int open;                  // +0x2c
    int optimized;             // +0x30
    int unknown34;
    unsigned char unknown38[0x40 - 0x38];
};
class CAreaSearchNodeQueue {
  public:
    void Clear();
};
class CAreaSearchNodeList : public CLinkedList {
  public:
    CAreaSearchNodeList();
};
#endif
