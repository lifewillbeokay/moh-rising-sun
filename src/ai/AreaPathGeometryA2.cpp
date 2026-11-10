// AI-assisted reconstruction from GR8E69. Fragments of the original AreaPathNode.cpp;
// function and variable names come from the symbol table, member names are inferred.
// See docs/Behaviors.md.
#include "AISplinePath.h"
#include "CLine2.h"
extern "C" float sqrtf(float);

bool CAreaPathNode::LineIntersectsBorderEdges(const CLine2 &line, CVector2 *hit) const {
    int linkIndex = 0;
    int count = linkCount + borderVertexCount;
    for (int i = 0; i < count; i++) {
        if (linkIndex < linkCount && links[linkIndex].vertex0 == i) {
            linkIndex++;
            continue;
        }
        CVector2 a, b;
        a.Set(g_pAreaVertices[vertexIndices[i]].x, g_pAreaVertices[vertexIndices[i]].y);
        b.Set(g_pAreaVertices[vertexIndices[(i + 1) % count]].x, g_pAreaVertices[vertexIndices[(i + 1) % count]].y);
        CLine2 edge(a, b);
        g_BorderLine = edge;
        if (line.Intersect(g_BorderLine, 0.01f, hit))
            return true;
    }
    return false;
}

CAreaPathNode *CAreaPathNode::LineIntersectsLinkEdges(const CLine2 &line, CVector2 *hit, CAreaPathNode *except) const {
    for (int i = 0; i < linkCount; i++) {
        CVector2 a, b;
        a.Set(g_pAreaVertices[vertexIndices[links[i].vertex0]].x, g_pAreaVertices[vertexIndices[links[i].vertex0]].y);
        b.Set(g_pAreaVertices[vertexIndices[links[i].vertex1]].x, g_pAreaVertices[vertexIndices[links[i].vertex1]].y);
        CLine2 edge(a, b);
        if (line.Intersect(edge, 0.000001f, hit)) {
            CAreaPathNode *node = AIPathFinding::GetPathNodeByIndex(links[i].NodeIndex());
            if (!except || node != except)
                return node;
        }
    }
    return 0;
}
