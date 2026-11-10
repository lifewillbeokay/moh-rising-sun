// AI-assisted reconstruction from GR8E69. Fragments of the original AreaPathNode.cpp;
// function and variable names come from the symbol table.
#include "AISplinePath.h"

extern CAreaPathNode *g_pPathFindingAreas;
extern int g_iNumPathFindingAreas;
namespace AIPathFinding {
static int g_iLastPathNodeUpdatedOutOfArea = 0;
}

CAreaPathNode *AIPathFinding::GetPathNodeByIndex(unsigned short index) { return &g_pPathFindingAreas[index]; }
unsigned short AIPathFinding::GetIndexByPathNode(CAreaPathNode *node) { return node - g_pPathFindingAreas; }

bool AIPathFinding::IsPointOnNetwork(const CVector3 &point) {
    CAreaPathNode *node = GetClosestPathNode(point, -1);
    if (node)
        return node->ContainsPoint(point);
    return false;
}

void AIPathFinding::UpdateAStarNodesOutOfArea() {
    if (g_iNumPathFindingAreas) {
        for (int i = 0; i < 50; i++) {
            g_iLastPathNodeUpdatedOutOfArea = (g_iLastPathNodeUpdatedOutOfArea + 1) % g_iNumPathFindingAreas;
            g_pPathFindingAreas[g_iLastPathNodeUpdatedOutOfArea].UpdateOutOfArea();
        }
    }
}

void AIPathFinding::ResetAreaPathNodes() {
    g_iLastPathNodeUpdatedOutOfArea = 0;
    for (int i = 0; i < g_iNumPathFindingAreas; i++)
        g_pPathFindingAreas[i].ResetOutOfArea();
}
