// AI-assisted reconstruction from GR8E69. Fragments of the original AreaPathNode.cpp;
// function and variable names come from the symbol table, member names are inferred.
// See docs/Behaviors.md.
#include "AISplinePath.h"
#include "CLine2.h"
extern "C" float sqrtf(float);

bool CAreaPathNode::GetClosestPointOnBorder(const CVector3 &point, CVector3 &out) const {
    if (ContainsPoint(point)) {
        out = point;
        return false;
    }
    float best = 10000000.0f;
    int found = 0;
    CVector2 closest;
    int count = linkCount + borderVertexCount;
    for (int i = 0; i < count; i++) {
        CVector2 a, b;
        a.Set(g_pAreaVertices[vertexIndices[i]].x, g_pAreaVertices[vertexIndices[i]].y);
        b.Set(g_pAreaVertices[vertexIndices[(i + 1) % count]].x, g_pAreaVertices[vertexIndices[(i + 1) % count]].y);
        CLine2 edge(a, b);
        CVector2 p, hit;
        p.Set(point.x, point.y);
        float distance = edge.GetClosestPointSquared(p, &hit);
        if (distance < best || !found) {
            best = distance;
            closest = hit;
            found = 1;
        }
    }
    out.Set(closest.x, closest.y, point.z);
    return true;
}

float CAreaPathNode::GetFurthestVertex(const CVector3 &from, const CVector3 &to, CVector3 *out, bool nearTo) const {
    float best = -1.0f;
    int count = linkCount + borderVertexCount;
    for (int i = 0; i < count; i++) {
        CVector3 v(g_pAreaVertices[vertexIndices[i]].x, g_pAreaVertices[vertexIndices[i]].y, 0.0f);
        float toDistance = v.DistanceSquared(to);
        float fromDistance = v.DistanceSquared(from);
        if (!nearTo) {
            if (toDistance < fromDistance)
                continue;
        }
        if (toDistance > best || !(best >= 0.0f)) {
            if (out)
                *out = v;
            best = toDistance;
        }
    }
    return best;
}

bool CAreaPathNode::IsEdgeConnected(int vertex) const {
    for (int i = 0; i < linkCount; i++) {
        if (links[i].vertex0 == vertex)
            return true;
    }
    return false;
}

int AreaPathBSP::GetCurrentNode(const CVector3 &point, int network) {
    BPDPathFindingBSPBranch *planes = AIPathFinding::GetBSP(network);
    if (!planes)
        return -1;
    int node = 0;
    do {
        if (!(planes[node].a * point.x + planes[node].b * point.y + planes[node].c * point.z + planes[node].d <= 0.0f))
            node = planes[node].front;
        else
            node = planes[node].back;
    } while (node > 0);
    return ~node;
}

CVector3 CAreaPathNode::GetCentroid() const {
    CVector3 sum;
    sum.Zero();
    int count = borderVertexCount + linkCount;
    if (count <= 2)
        return sum;
    for (int i = 0; i < count; i++) {
        CVector3 v;
        v.Set(g_pAreaVertices[vertexIndices[i]].x, g_pAreaVertices[vertexIndices[i]].y, g_pAreaVertices[vertexIndices[i]].z);
        sum += v;
    }
    sum *= 1.0f / count;
    return sum;
}

void CAreaPathNode::BiasTowardCentroid(CVector3 *point, float distance) const {
    const CVector3 &centroid = GetCentroid();
    CVector3 d;
    d.Set(centroid.x - point->x, centroid.y - point->y, centroid.z - point->z);
    if (!(sqrtf(d.LengthSquared()) >= distance)) {
        *point = centroid;
    } else {
        float length = sqrtf(d.LengthSquared());
        if (length != 0.0f)
            d *= 1.0f / length;
        d *= distance;
        *point += d;
    }
}

float CAreaPathNode::GetClosestPointSquared(const CVector3 &point, CVector3 *out) const {
    CVector3 closest;
    float best = 1000000000.0f;
    int count = borderVertexCount + linkCount;
    for (int i = 0; i < count; i++) {
        CVector2 a, b, p, hit;
        int index = vertexIndices[i];
        int next = vertexIndices[(i + 1) % count];
        a.Set(g_pAreaVertices[index].x, g_pAreaVertices[index].y);
        b.Set(g_pAreaVertices[next].x, g_pAreaVertices[next].y);
        CLine2 edge(a, b);
        p.Set(point.x, point.y);
        float distance = edge.GetClosestPointSquared(p, &hit);
        if (!(distance >= best)) {
            best = distance;
            closest.Set(hit.x, hit.y, g_pAreaVertices[index].z);
        }
    }
    if (out) {
        CVector3 toCentroid = GetCentroid();
        toCentroid -= closest;
        float length = sqrtf(toCentroid.LengthSquared());
        if (length != 0.0f)
            toCentroid *= 1.0f / length;
        toCentroid *= 0.01f;
        closest += toCentroid;
        *out = closest;
    }
    return best;
}

CAreaPathNode *CAreaPathNode::GetCloserLink(const CVector3 &point) const {
    CAreaPathNode *best = 0;
    float bestDistance = GetClosestPointSquared(point, 0);
    for (int i = 0; i < linkCount; i++) {
        CAreaPathNode *node = AIPathFinding::GetPathNodeByIndex(links[i].NodeIndex());
        float distance = node->GetClosestPointSquared(point, 0);
        if (!(distance >= bestDistance)) {
            bestDistance = distance;
            best = node;
        }
    }
    return best;
}
