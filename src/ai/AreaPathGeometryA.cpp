// AI-assisted reconstruction from GR8E69. Fragments of the original AreaPathNode.cpp;
// function and variable names come from the symbol table, member names are inferred.
// See docs/Behaviors.md.
#include "AISplinePath.h"
#include "CLine2.h"
extern "C" float sqrtf(float);

float CAreaPathNode::GetClosestPosition(const CVector3 &point, CVector3 &out) const {
    float best = 0.0f;
    const PropVec3 *closest = 0;
    int count = linkCount + borderVertexCount;
    for (int i = 0; i < count; i++) {
        const PropVec3 &a = g_pAreaVertices[vertexIndices[i]];
        CVector3 v(a.x, a.y, a.z);
        float distance = v.DistanceSquaredXY(point);
        if (!closest || !(distance >= best)) {
            best = distance;
            closest = &g_pAreaVertices[vertexIndices[i]];
        }
    }
    out.Set(closest->x, closest->y, closest->z);
    return sqrtf(best);
}

bool CAreaPathNode::ContainsPoint(const CVector3 &point) const {
    int side = 0, sideSet = 0;
    int count = linkCount + borderVertexCount;
    for (int i = 0; i < count; i++) {
        const PropVec3 &a = g_pAreaVertices[vertexIndices[i]];
        const PropVec3 &b = g_pAreaVertices[vertexIndices[(i + 1) % count]];
        float cross = (point.y - a.y) * (b.x - a.x) - (point.x - a.x) * (b.y - a.y);
        if (__builtin_fabsf(cross) < 0.1f)
            continue;
        int positive = cross >= 0.0f;
        if (sideSet) {
            if (side != positive)
                return false;
        } else {
            side = positive;
            sideSet = 1;
        }
    }
    return true;
}

bool CAreaPathNode::GetEdgeVertices(CAreaPathNode *other, CVector3 &a, CVector3 &b) const {
    int index = AIPathFinding::GetIndexByPathNode(other);
    for (int i = 0; i < linkCount; i++) {
        if (index == links[i].NodeIndex()) {
            PropVec3 v = g_pAreaVertices[vertexIndices[links[i].vertex0]];
            a.Set(v.x, v.y, v.z);
            v = g_pAreaVertices[vertexIndices[links[i].vertex1]];
            b.Set(v.x, v.y, v.z);
            return true;
        }
    }
    return false;
}
