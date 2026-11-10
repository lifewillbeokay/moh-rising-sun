// AI-assisted reconstruction of the projector system definition; see docs/Rendering.md.
#include "TARExtension.h"
#include "Toilet.h"
#include "EAGLMemory.h"
#include "ProjectorSystemDef.h"
void *TLT_LoadFromRepository(const char *, int *, bool *);
// The TAR view declares no allocator; this is what its allocation compiles to.
CProjectorSystemDef::CProjectorSystemDef() : resource(0), file(0), texture(0) {
}
CProjectorSystemDef::~CProjectorSystemDef() {
    delete texture;
    texture = 0;
    file = 0;
}
CProjectorSystemDef::CProjectorSystemDef(const CProjectorSystemDef &other)
    : resource(other.resource), file(other.file), texture(other.texture) {
}
CProjectorSystemDef::CProjectorSystemDef(LevelFileContentsStruct_ *contents) {
    resource = contents;
    int size;
    bool flag;
    char *data = (char *)TLT_LoadFromRepository((const char *)resource->data[1]->values[0], &size, &flag);
    SHAPE *shape = (SHAPE *)(data + *(int *)(data + 20));
    EAGL::TAR *tar = new (EAGLInternal::EAGLMalloc(sizeof(EAGL::TAR), 0)) EAGL::TAR(shape);
    tar->SetClampMode(EAGL::CM_CLAMP);
    file = data;
    texture = tar;
}
