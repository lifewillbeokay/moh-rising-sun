// Scoped declarations for the retained game-container operations only.
// Original type names come from symbols. Storage bounds, copies, call signatures
// and comparator behavior come from original instructions; see docs/GameContainers.md.
// Unresolved byte ranges may include fields and padding. Their semantic types and
// original member names are not asserted. Do not use these shells as complete
// gameplay class declarations or combine them with other definitions of the types.
#ifndef MOH_GAME_CONTAINER_TYPES_H
#define MOH_GAME_CONTAINER_TYPES_H

#include <string>
#include <list>

extern "C" int strcasecmp(const char*, const char*);

namespace EALA { namespace Character {
// These enclosing classes provide nested names only; their own sizes are unknown.
class SkeletonData {
public:
    struct AttachPoint {
        unsigned char unresolved_00[8] __attribute__((aligned(4)));
    };
    struct Compare {
        bool operator()(const _STL::string& a, const _STL::string& b) const {
            return a.compare(b) < 0;
        }
    };
};
class Mesh {
public:
    struct Geometry {
        unsigned char unresolved_00[12] __attribute__((aligned(4)));
    };
    struct StringCompare {
        bool operator()(const char* a, const char* b) const {
            return strcasecmp(a, b) < 0;
        }
    };
};
}}

class AnimShape {
    // Copies and destruction call the original nontrivial routines.
    unsigned char unresolved_00[60] __attribute__((aligned(4)));
public:
    struct InitParams {
        unsigned char unresolved_00[8] __attribute__((aligned(4)));
    };
    AnimShape(const AnimShape&);
    ~AnimShape();
};
class StaticMesh;
class CFont {
public:
    struct FontRef {
        unsigned char unresolved_00[12] __attribute__((aligned(4)));
    };
};
class CCompartment {
public:
    class CInstanceModel {
        unsigned char unresolved_00[32] __attribute__((aligned(4)));
    public:
        CInstanceModel(const CInstanceModel&);
        ~CInstanceModel();
    };
};
class HandlerLeaderboard {
public:
    struct Player {
        unsigned char unresolved_00[12] __attribute__((aligned(4)));
        int field_0c, field_10, field_14, field_18, field_1c;
    };
};

struct PathFindingNetworkLink {
    unsigned char unresolved_00[20] __attribute__((aligned(4)));
};
class PathFindingNetwork {
    unsigned char field_00; // Copied byte; semantic name remains unresolved.
    _STL::list<PathFindingNetworkLink> links; // Observed member at offset 4.
public:
    PathFindingNetwork(const PathFindingNetwork&);
};

class SHAPE;
namespace EAGL {
class Model {
public:
    struct TARList {
        unsigned char unresolved_00[20] __attribute__((aligned(4)));
    };
};
}
struct _LocationTargetInfo {
    unsigned char unresolved_00[12] __attribute__((aligned(4)));
};
struct BSObject;
// Message registration/removal independently establish these fields; names are
// descriptive. The final two bytes remain opaque and are not initialized there.
struct BSMessageToEventRecord {
    BSObject *object;
    unsigned short event;
    unsigned char unknown06[2];
};
class CProjectorSystemDef {
    unsigned char unresolved_00[12] __attribute__((aligned(4)));
public:
    CProjectorSystemDef(const CProjectorSystemDef&);
    ~CProjectorSystemDef();
};

// Check the representation required by the retained original operations.
#define MOH_CONTAINER_SIZE(name, type, bytes) typedef char name[(sizeof(type) == bytes) ? 1 : -1]
MOH_CONTAINER_SIZE(AttachPointStorage, EALA::Character::SkeletonData::AttachPoint, 8);
MOH_CONTAINER_SIZE(GeometryStorage, EALA::Character::Mesh::Geometry, 12);
MOH_CONTAINER_SIZE(InitParamsStorage, AnimShape::InitParams, 8);
MOH_CONTAINER_SIZE(AnimShapeStorage, AnimShape, 60);
MOH_CONTAINER_SIZE(FontRefStorage, CFont::FontRef, 12);
MOH_CONTAINER_SIZE(InstanceModelStorage, CCompartment::CInstanceModel, 32);
MOH_CONTAINER_SIZE(LeaderboardPlayerStorage, HandlerLeaderboard::Player, 32);
MOH_CONTAINER_SIZE(NetworkLinkStorage, PathFindingNetworkLink, 20);
MOH_CONTAINER_SIZE(NetworkStorage, PathFindingNetwork, 12);
MOH_CONTAINER_SIZE(TARListStorage, EAGL::Model::TARList, 20);
MOH_CONTAINER_SIZE(LocationTargetStorage, _LocationTargetInfo, 12);
MOH_CONTAINER_SIZE(MessageRecordStorage, BSMessageToEventRecord, 8);
MOH_CONTAINER_SIZE(ProjectorDefStorage, CProjectorSystemDef, 12);
#undef MOH_CONTAINER_SIZE

#endif
