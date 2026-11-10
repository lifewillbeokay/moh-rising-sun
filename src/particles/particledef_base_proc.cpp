// AI-assisted reconstruction of the particle definition base classes; see docs/ParticleRecipes.md.
#include "TARExtension.h"
#include "StaticMesh.h"
#include "ParticleDef.h"
void *TLT_LoadFromRepository(const char *, int *, bool *);
CParticleDef::CParticleDef() {}
CParticleDef::~CParticleDef() {}
void CParticleDef::Init(LevelFileContentsStruct_ *c) { contents = c; }
bool CParticleDef::IsValid() { return contents != 0; }
CProcParticleDef::CProcParticleDef() {}
CProcParticleDef::~CProcParticleDef() {
    if (texture) {
        delete texture;
        texture = 0;
    }
}
void CProcParticleDef::Init(LevelFileContentsStruct_ *c) {
    CParticleDef::Init(c);
    unknown_0 = 1;
    int size;
    bool inRepository;
    char *data = (char *)TLT_LoadFromRepository(contents->entries[2]->value.text, &size, &inRepository);
    const SHAPE *shape = (const SHAPE *)(data + *(int *)(data + 20));
    EAGL::TAR *tar = new (EAGLInternal::EAGLMalloc(sizeof(EAGL::TAR), 0)) EAGL::TAR(shape);
    tar->SetClampMode(EAGL::CM_CLAMP);
    shapeFile = data;
    texture = tar;
    SetEmitDelay(contents->entries[4]->value.integer);
    SetEmitRate(contents->entries[3]->value.integer);
    SetLocMin(contents->entries[7]->value.scalar, contents->entries[8]->value.scalar, contents->entries[9]->value.scalar);
    SetLocMax(contents->entries[10]->value.scalar, contents->entries[11]->value.scalar, contents->entries[12]->value.scalar);
    SetVelMin(contents->entries[13]->value.scalar, contents->entries[14]->value.scalar, contents->entries[15]->value.scalar);
    SetVelMax(contents->entries[16]->value.scalar, contents->entries[17]->value.scalar, contents->entries[18]->value.scalar);
}
