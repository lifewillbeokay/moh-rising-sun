// A fragment of the original AnimObjFactory.cpp: loading a player weapon's definition from the level's
// contents. Class and method names come from the symbol table; the definition's members and
// the table indices' meanings are inferred from this code.
#include "profile.h"
#include "ResourceTypes.h"
#include "Toilet.h"

struct ShapeFile;

namespace EALA {
namespace Character {
class SkeletonData : public Resource {
public:
    static Resource::Pointer<SkeletonData> Alloc(const char *);
};
class Mesh : public Resource {
public:
    static Resource::Pointer<Mesh> Alloc(const char *, bool);
    void Hide(const char *);
    void Bind(ShapeFile *);
};
}
}

class CAnimObjDef {
public:
    unsigned char unknown00[72];
    EALA::Resource::Pointer<EALA::Character::SkeletonData> skeleton;
    EALA::Resource::Pointer<EALA::Character::Mesh> body;
    EALA::Resource::Pointer<EALA::Character::Mesh> head;
};

class CAnimObjectFactory {
public:
    void LoadSkeleton(LevelFileContentsStruct_ *contents, CAnimObjDef *definition);
    void LoadBody(LevelFileContentsStruct_ *contents, CAnimObjDef *definition);
    void LoadPlayerWeapon(LevelFileContentsStruct_ *contents, CAnimObjDef *definition);
    void LoadHead(LevelFileContentsStruct_ *contents, CAnimObjDef *definition);
    void LoadTextures(LevelFileContentsStruct_ *contents, unsigned int table, EALA::Resource::Pointer<EALA::Character::Mesh> &mesh);
};

void CAnimObjectFactory::LoadPlayerWeapon(LevelFileContentsStruct_ *contents, CAnimObjDef *definition) {
    definition->skeleton = EALA::Character::SkeletonData::Alloc((const char *)contents->data[0]->value);
    definition->body = EALA::Character::Mesh::Alloc((const char *)contents->data[1]->value, true);
    LoadTextures(contents, 5, definition->body);
}
