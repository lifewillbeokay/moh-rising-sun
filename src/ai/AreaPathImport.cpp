// AI-assisted reconstruction from GR8E69. Fragments of the original AreaPathNode.cpp;
// function and variable names come from the symbol table, member names are inferred.
// See docs/Behaviors.md.
#include "AISplinePath.h"
#include "Endian.h"
#include "OffsetPtr.h"

BPDPathFindingBSPBranch *AIPathFinding::GetBSP(int network) {
    for (int i = 0; i < g_iNumPathFindingBSP; i++) {
        if (g_pPathFindingBSPs[i].network == network)
            return g_pPathFindingBSPs[i].branches;
    }
    return 0;
}

void AIPathFinding::ImportAreaPathNodeNetwork(BPDHeader *header) {
    offsetPtr(header->areas, (int)header);
    offsetPtr(header->points, (int)header);
    offsetPtr(header->bsp, (int)header);
    g_pAreaVertices = header->points;
    g_iNumAreaVertices = header->pointCount;
    g_pPathFindingAreas = (CAreaPathNode *)header->areas;
    g_iNumPathFindingAreas = header->areaCount;
    g_pPathFindingBSPs = header->bsp;
    g_iNumPathFindingBSP = header->bspCount;
    for (int v = 0; v < g_iNumAreaVertices; v++) {
        PropVec3 &point = g_pAreaVertices[v];
        ChangeEndian(point.x);
        ChangeEndian(point.y);
        ChangeEndian(point.z);
    }
    for (int a = 0; a < g_iNumPathFindingAreas; a++) {
        CAreaPathNode &area = g_pPathFindingAreas[a];
        ChangeEndian(area.links);
        ChangeEndian(area.vertexIndices);
        offsetPtr(area.vertexIndices, (int)header);
        offsetPtr(area.links, (int)header);
        int count = area.linkCount + area.borderVertexCount;
        for (int j = 0; j < count; j++)
            ChangeEndian(area.vertexIndices[j]);
        for (int k = 0; k < area.linkCount; k++)
            ChangeEndian(area.links[k].nodeAndType);
    }
    for (int b = 0; b < g_iNumPathFindingBSP; b++) {
        ChangeEndian(g_pPathFindingBSPs[b].branches);
        ChangeEndian(g_pPathFindingBSPs[b].branchCount);
        offsetPtr(g_pPathFindingBSPs[b].branches, (int)header);
        for (int j = 0; j < g_pPathFindingBSPs[b].branchCount; j++) {
            BPDPathFindingBSPBranch &branch = g_pPathFindingBSPs[b].branches[j];
            ChangeEndian(branch.back);
            ChangeEndian(branch.front);
            ChangeEndian(branch.a);
            ChangeEndian(branch.b);
            ChangeEndian(branch.c);
            ChangeEndian(branch.d);
        }
    }
}

CAreaPathNode *AIPathFinding::GetClosestPathNode(const CVector3 &point, int network) {
    if (!g_pPathFindingAreas)
        return 0;
    if (network < 0)
        network = CAIAreaPathFinding::WhichNetwork(point);
    int index = AreaPathBSP::GetCurrentNode(point, network);
    if (index < 0 || index > g_iNumPathFindingAreas)
        return 0;
    return &g_pPathFindingAreas[index];
}
