#ifndef GAME_PROJECTOR_SYSTEM_DEF_H
#define GAME_PROJECTOR_SYSTEM_DEF_H
// AI-assisted scoped view of CProjectorSystemDef; see docs/Rendering.md. Member names are inferred.
// include/game-containers holds a storage-only view of this class for the map's instantiations.
struct LevelFileContentsStruct_;
namespace EAGL {
class TAR;
}
class CProjectorSystemDef {
  public:
    LevelFileContentsStruct_ *resource;
    char *file;         // +4
    EAGL::TAR *texture; // +8
    CProjectorSystemDef();
    ~CProjectorSystemDef();
    CProjectorSystemDef(const CProjectorSystemDef &);
    CProjectorSystemDef(LevelFileContentsStruct_ *);
    // Assignment takes over the other definition's resources.
    void operator=(CProjectorSystemDef &other) {
        LevelFileContentsStruct_ *r = other.resource;
        char *f = other.file;
        EAGL::TAR *t = other.texture;
        resource = r;
        file = f;
        texture = t;
        other.texture = 0;
        other.file = 0;
        other.resource = 0;
    }
};
#endif
