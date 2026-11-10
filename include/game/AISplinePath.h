#ifndef GAME_AI_SPLINE_PATH_H
#define GAME_AI_SPLINE_PATH_H
#include "AIFilterGlobal.h"
#include "BPD.h"
#include "AIMovement.h"
#include "LinkedList.h"
// AI-assisted reconstruction from GR8E69; see docs/Paths.md.
class CAISplinePathManager;
class CAISplinePathModule;
struct BSObject;
class TriggerObject;
// Original enum tag; its members are unknown.
enum EAStarDestinationInfo { AStarDestinationUnknown = 0 };
// Eighty-byte array stride, independently observed in generation/destruction.
// Coefficient names and parameterStep describe their observed use.
class CAISplinePathSegment {
public:
    unsigned char unknown00[4];
    float parameterStep;
    unsigned char unknown08[8];
    CVector3 cubic, quadratic, linear, constant;
    CAISplinePathSegment();
    ~CAISplinePathSegment();
    void Set(float, const CVector3 &, const CVector3 &, const CVector3 &, const CVector3 &);
    void Expand(float, CVector3 &);
    void ExpandDerivative(float, CVector3 &);
    void ExpandSecondDerivative(float, CVector3 &);
    float GetClosestParameter(float, const CVector3 &);
};
// The original array construction, destruction and lookup establish a 12-byte
// stride. Generation writes input point count minus one at +4; GetLastPoint
// indexes that entry and evaluates it at zero. Field names are descriptive.
class CAISplinePath {
public:
    CAISplinePathSegment *segments;
    unsigned int lastSegment;
    unsigned int id;
    CAISplinePath();
    ~CAISplinePath();
    void GetLastPoint(CVector3 *);
    void GenerateTestSplinePath(const CAISplinePathManager &, unsigned int, const PropVec3 *, bool);
};
// Its allocation and six constructor stores establish 24 bytes. Buffer element
// types remain unspecified here; original spline math consumes them.
class CAISplinePathManager {
public:
    unsigned int pathCount;
    CAISplinePath *paths;
    void *buffer08, *buffer0c, *buffer10, *buffer14;
    CAISplinePathManager(unsigned int);
    ~CAISplinePathManager();
    void AllocateGenerateBuffers();
    void FreeGenerateBuffers();
    void GenerateSplinePath(unsigned int, unsigned int, unsigned int, const PropVec3 *);
};
// Observed vector prefix only; no complete allocation size is established.
class CAISplinePathBreakdownPoint {
public:
    CVector3 position;
    CAISplinePathBreakdownPoint();
    ~CAISplinePathBreakdownPoint();
    bool WasCrossedDistance(const CVector3 &, float *);
};
// Observed prefix through the locomotion pointer at +0x20. Constructor and
// traversal accesses agree; this is not the complete navigation class.
class CAIObject;
#include "CLine2.h"
#include "BPD.h"
// One link from an area node to a neighbor (four bytes, from the stride of the link walks).
struct BPDLinkEdge {
    unsigned short nodeAndType; // The low 14 bits are the neighbor's node index.
    unsigned char vertex0;      // +0x02; index into the owner's vertex indices.
    unsigned char vertex1;      // +0x03
    int NodeIndex() const { return nodeAndType & 0x3fff; }
};
// One splitting plane of an area BSP (24 bytes, from the stride).
struct BPDPathFindingBSPBranch {
    float a, b, c, d;
    int front; // +0x10
    int back;  // +0x14
};
// One BSP per network (twelve bytes, from the stride).
struct BPDPathFindingBSP {
    BPDPathFindingBSPBranch *branches;
    int branchCount;
    unsigned char network; // +0x08
};
extern PropVec3 *g_pAreaVertices;
// A* area node. Sixteen bytes; the member names are inferred from the geometry functions.
class CAreaPathNode {
public:
    float GetFurthestVertex(const CVector3 &, const CVector3 &, CVector3 *, bool) const;
    void BiasTowardCentroid(CVector3 *, float) const;
    bool ContainsPoint(const CVector3 &) const;
    void UpdateOutOfArea();
    void ResetOutOfArea();
    void MarkAsOutOfArea();
    void MarkAsInArea();
    bool GetEdgeVertices(CAreaPathNode *, CVector3 &, CVector3 &) const;
    bool IsEdgeConnected(int) const;
    CAreaPathNode *GetCloserLink(const CVector3 &) const;
    CVector3 GetCentroid() const;
    float GetClosestPointSquared(const CVector3 &, CVector3 *) const;
    float GetClosestPosition(const CVector3 &, CVector3 &) const;
    CAreaPathNode *LineIntersectsLinkEdges(const CLine2 &, CVector2 *, CAreaPathNode *) const;
    int GetNextNode(CLine2 &, int) const;
    bool LineEndsAtLinkEdge(const CLine2 &, CAreaPathNode *) const;
    bool GetClosestPointOnBorder(const CVector3 &, CVector3 &) const;
    bool LineIntersectsBorderEdges(const CLine2 &, CVector2 *) const;
    class CAreaSearchNode *searchNode; // +0x00
    unsigned char borderVertexCount; // +0x04
    unsigned char linkCount;         // +0x05
    unsigned char inArea;            // +0x06
    unsigned char network;           // +0x07
    unsigned short *vertexIndices;   // +0x08; borderVertexCount + linkCount entries.
    BPDLinkEdge *links;                // +0x0c
};
class CAreaSearchNodeQueue;
class CAreaSearchNode;
class CAreaSearchNodeList;
// Member offsets come from the path walk functions; the names are inferred.
class CAIAreaPathFinding {
public:
    static void ResetAStarVolumes();
    unsigned char unknown00[4];
    int positionValid;        // +0x04; set with onNetwork by Update.
    int walkActive;           // +0x08
    unsigned char unknown0c[0x14 - 0x0c];
    CAreaPathNode *currentNode; // +0x14
    unsigned char unknown18[0x1c - 0x18];
    CAIPhysics *physics;      // +0x1c
    CAILocomotion *locomotion; // +0x20
    BSObject *scriptObject;   // +0x24
    int onNetwork;            // +0x28
    int currentVolume;        // +0x2c
    int volumeCount;          // +0x30
    unsigned char unknown34[0x40 - 0x34];
    CVector3 closestPosition; // +0x40
    CAIObject *targetObject;  // +0x50
    unsigned char unknown54[0x70 - 0x54];
    CVector3 destination;     // +0x70
    int field80;              // +0x80
    CLinkedListElement *currentPathNode; // +0x84
    CLinkedList solutionPath; // +0x88
    float commitTime;         // +0x94
    int pathFinished;         // +0x98
    unsigned char unknown9c[0xa4 - 0x9c];
    int fieldA4;              // +0xa4
    int fieldA8;              // +0xa8
    int fieldAC;              // +0xac
    unsigned char unknownb0[0xb4 - 0xb0];
    int walkPhase;            // +0xb4
    void SetupAStarPathWalkCommit();
    void TrimSolutionNodeQueue(CAreaSearchNode *, CAreaSearchNodeList &);
    void TeleportToNextPositionOnPath();
    int GetNextWalkToPos(CVector3 &);
    void ClearSearchNodeQueue(CAreaSearchNodeQueue &);
    void ClearSearchNodeList(CAreaSearchNodeList &);
    void CleanupAStarPathWalk();
    void StopAStarPathWalk();
    float PathDistance() const;
    void UpdateOffAStarPosition();
    int StartAStarPathWalk(BSObject *, EAStarDestinationInfo);
    int StartAStarPathWalk(EAStarDestinationInfo);
    void EnterAStarVolume(int);
    void GetClosestPositionOnAStar(CVector3 *) const;
    void FinishPath();
    int GetWalkType();
    void StopWalk();
    static int WhichNetwork(const CVector3 &);
    static void RegisterAStarVolume(TriggerObject *, int);
    static void RegisterAStarLinkPoint(TriggerObject *, int, int, int);
    void ExitAStarVolume(int);
    void UpdateAStarPathWalk();
    void WalkAStarPath();
    bool SetupAStarPathWalkForPatrol(const CVector3 &);
    void RemovePointersTo(CAIObject *);
    CAreaPathNode *GreedySearchAwayFromPoint(const CVector3 &, float, bool);
    void Update();
};
// Static navigation-mesh query. A namespace, not a class: ProDG mangles a no-argument
// function in a namespace with a trailing `v` (`ResetAreaPathNodes__13AIPathFindingv`) and a
// no-argument class static without one (`f__1A`), and `_13AIPathFinding.g_iLastPathNodeUpdatedOutOfArea`
// has local binding, which a class static data member cannot have.
namespace AIPathFinding {
void ResetAreaPathNodes();
    BPDPathFindingBSPBranch *GetBSP(int);
    void ImportAreaPathNodeNetwork(BPDHeader *);
    CAreaPathNode *GetClosestPathNode(const CVector3 &, int);
    CAreaPathNode *GetPathNodeByIndex(unsigned short);
    unsigned short GetIndexByPathNode(CAreaPathNode *);
    bool IsPointOnNetwork(const CVector3 &);
    void UpdateAStarNodesOutOfArea();
}
namespace AreaPathBSP {
    int GetCurrentNode(const CVector3 &, int);
}
extern CLine2 g_BorderLine;
extern CAreaPathNode *g_pPathFindingAreas;
extern int g_iNumPathFindingAreas;
extern int g_iNumAreaVertices;
extern BPDPathFindingBSP *g_pPathFindingBSPs;
extern int g_iNumPathFindingBSP;
// Prefix through the next-point vector at +0x20. The untouched bytes at
// +0x18 and the complete allocation size remain unestablished.
class CAISplinePathTraversal {
public:
    int state;
    float parameter, previousDistanceSquared;
    unsigned int segmentIndex;
    CAISplinePath *path;
    CAISplinePathSegment *segment;
    unsigned char unknown18[8];
    CAISplinePathBreakdownPoint nextPoint;
    CAISplinePathTraversal();
    ~CAISplinePathTraversal();
    float GetCurrentParameterValue(float, const CVector3 &);
    void MakeModifiedNextPoint();
    void SetupNextSegmentForward(const CVector3 &);
    void PrepareForTraversalForward(CAISplinePath *, bool, CAIAreaPathFinding *);
    void RestartForwardTraversal(CAIAreaPathFinding *);
    void ContinueTraversalForwardAStar(CAIPhysics *, CAIAreaPathFinding *);
    void GetNextPointForward(const CVector3 &, const CVector3 &, CVector3 *, BSObject *, CAISplinePathModule *, CAIAreaPathFinding *);
    void GetFinalPointForward(CVector3 *);
    void GetCurrentDerivative(CVector3 *);
};
// Prefix through the traversal member at +0x20; not a complete class allocation
// or virtual interface. Original dispatch storage at zero is left opaque.
// These fragments reconstruct direct calls; construction/registration stay original.
class CAISplinePathModule {
public:
    unsigned char unknown00[4];
    CAIPhysics *physics;
    CAIAreaPathFinding *areaPathFinding;
    CAILocomotion *locomotion;
    BSObject *scriptObject;
    unsigned char unknown14[12];
    CAISplinePathTraversal traversal;
    void StartSplinePathWalkForward(CAISplinePath *);
    void WalkNextSplinePathPoint();
    void StopSplinePathWalk();
    void ContinueSplinePathWalk();
    void LoopSplinePathWalk();
    int GetWalkType();
    void StopWalk();
};
// Inline id lookup used by BIFunc_PlayerStartPath. A missing id yields the
// slot one past the last path, as in the original.
inline CAISplinePath *FindSplinePath(unsigned int id) {
    CAISplinePathManager *manager = g_aigAIFilterGlobalObject.splineManager;
    unsigned int i;
    for (i = 0; i < manager->pathCount; i++)
        if (manager->paths[i].id == id)
            break;
    return &manager->paths[i];
}
#endif
