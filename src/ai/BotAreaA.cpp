// AI-assisted reconstruction from GR8E69. Fragments of the original source/BOT/BotArea.cpp;
// function and variable names come from the symbol table, member names are inferred.
#include "BotArea.h"
extern "C" {
void *DWI_alloc(const char *, int, int);
int DWI_free(void *);
const char *DWI_getallocid();
void DWI_setallocid(const char *);
}

// Saves and restores the allocator's tag around a scope; the class name is inferred.
class DwiAllocIdScope {
  public:
    DwiAllocIdScope(const char *id) {
        saved = DWI_getallocid();
        DWI_setallocid(id);
    }
    ~DwiAllocIdScope() { DWI_setallocid(saved); }
    const char *saved;
};
static CBotArea *s_BotAreas = 0;
static CAreaEdge *s_AreaEdges = 0;

void InitBotAreaData() {
    DwiAllocIdScope scope("InitBotAreaData");
    int size = g_iNumPathFindingAreas * sizeof(CBotArea);
    if (!s_BotAreas)
        s_BotAreas = (CBotArea *)DWI_alloc("source/BOT/BotArea.cpp:49", size, 1024);
    size = g_iNumAreaVertices * sizeof(CAreaEdge);
    if (!s_AreaEdges)
        s_AreaEdges = (CAreaEdge *)DWI_alloc("source/BOT/BotArea.cpp:55", size, 1024);
}

void CleanBotAreaData() {
    if (s_BotAreas) {
        DWI_free(s_BotAreas);
        s_BotAreas = 0;
    }
    if (s_AreaEdges) {
        DWI_free(s_AreaEdges);
        s_AreaEdges = 0;
    }
}

CBotArea *GetBotArea(short index) { return &s_BotAreas[index]; }
CBotArea *GetBotArea(CAreaPathNode *node) { return &s_BotAreas[AIPathFinding::GetIndexByPathNode(node)]; }
CAreaEdge *GetAreaEdge(int index) { return &s_AreaEdges[index]; }

CAreaEdge *GetCoverEdge(int index) {
    CAreaEdge *edge = &s_AreaEdges[index];
    if (edge->coverData && s_AreaEdges[index].coverPoints > 0)
        return edge;
    return 0;
}

CAreaEdge *GetCoverEdge(int index, int vertex) {
    CAreaEdge *edge = &s_AreaEdges[index];
    if (edge->vertex1 == vertex && edge->coverData && s_AreaEdges[index].coverPoints > 0)
        return edge;
    return 0;
}

void CAreaEdge::GetEdgePoints(CVector3 &a, CVector3 &b) {
    a.Set(g_pAreaVertices[vertex0].x, g_pAreaVertices[vertex0].y, g_pAreaVertices[vertex0].z);
    b.Set(g_pAreaVertices[vertex1].x, g_pAreaVertices[vertex1].y, g_pAreaVertices[vertex1].z);
}
