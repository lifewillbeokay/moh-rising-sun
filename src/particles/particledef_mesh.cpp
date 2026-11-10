// AI-assisted reconstruction of the mesh particle definition; see docs/ParticleRecipes.md.
#include "TARExtension.h"
#include "StaticMesh.h"
#include "ParticleDef.h"
void *TLT_LoadFromRepository(const char *, int *, bool *);
extern "C" void *DWI_alloc(const char *, int, int);
// Parameter entries are in the recipe's list; see docs/ParticleRecipes.md for their indices.
CMeshParticleDef::CMeshParticleDef() {}
CMeshParticleDef::~CMeshParticleDef() {
    if (mesh) {
        delete mesh;
        mesh = 0;
    }
}
void CMeshParticleDef::Init(LevelFileContentsStruct_ *c) {
    CParticleDef::Init(c);
    unknown_0 = 2;
    int size;
    bool inRepository;
    StaticMesh *data = (StaticMesh *)TLT_LoadFromRepository(contents->entries[2]->value.text, &size, &inRepository);
    mesh = new (DWI_alloc("source/gfx/particledef.cpp:650", sizeof(CStaticMesh), 1024)) CStaticMesh(data, !inRepository);
    SetSystemLife(contents->entries[4]->value.scalar, contents->entries[5]->value.scalar);
    SetParticleLife(contents->entries[6]->value.scalar, contents->entries[7]->value.scalar);
    SetEmission(contents->entries[8]->value.scalar, contents->entries[9]->value.scalar);
    SetStartPos(contents->entries[10]->value.scalar, contents->entries[11]->value.scalar, contents->entries[12]->value.scalar);
    SetStartBox(contents->entries[13]->value.scalar, contents->entries[15]->value.scalar, contents->entries[14]->value.scalar);
    SetStartDir(contents->entries[16]->value.scalar, contents->entries[17]->value.scalar, contents->entries[18]->value.scalar, contents->entries[19]->value.scalar);
    SetStartSpeed(contents->entries[20]->value.scalar, contents->entries[21]->value.scalar);
    SetAccel(contents->entries[22]->value.scalar, contents->entries[23]->value.scalar, contents->entries[24]->value.scalar, contents->entries[25]->value.scalar);
    SetYaw(contents->entries[26]->value.scalar, contents->entries[27]->value.scalar, contents->entries[32]->value.scalar, contents->entries[33]->value.scalar);
    SetPitch(contents->entries[28]->value.scalar, contents->entries[29]->value.scalar, contents->entries[34]->value.scalar, contents->entries[35]->value.scalar);
    SetRoll(contents->entries[30]->value.scalar, contents->entries[31]->value.scalar, contents->entries[36]->value.scalar, contents->entries[37]->value.scalar);
    SetScale(contents->entries[38]->value.scalar, contents->entries[39]->value.scalar, contents->entries[40]->value.scalar, contents->entries[41]->value.scalar);
    SetStartColor(contents->entries[42]->value.scalar, contents->entries[43]->value.scalar, contents->entries[44]->value.scalar, contents->entries[45]->value.scalar);
    SetEndColor(contents->entries[46]->value.scalar, contents->entries[47]->value.scalar, contents->entries[48]->value.scalar, contents->entries[49]->value.scalar);
}
bool CMeshParticleDef::IsValid() { return contents != 0; }
void CMeshParticleDef::SetSystemLife(float life, float variance) {
    if (!(life >= 0.0f))
        life = 0.0f;
    if (!(variance >= 0.0f))
        variance = 0.0f;
    if (!(life - variance >= 0.0f))
        variance = life;
    contents->entries[4]->value.scalar = life;
    contents->entries[5]->value.scalar = variance;
}
void CMeshParticleDef::SetParticleLife(float life, float variance) {
    if (!(life > 0.0f))
        life = 0.001f;
    if (!(variance >= 0.0f))
        variance = 0.0f;
    if (!(life - variance >= 0.0f))
        variance = life;
    contents->entries[6]->value.scalar = life;
    contents->entries[7]->value.scalar = variance;
}
void CMeshParticleDef::SetEmission(float rate, float variance) {
    if (!(rate >= 0.001f))
        rate = 0.001f;
    if (!(variance >= 0.0f))
        variance = 0.0f;
    contents->entries[8]->value.scalar = rate;
    contents->entries[9]->value.scalar = variance;
}
void CMeshParticleDef::SetStartPos(float x, float y, float z) {
    contents->entries[10]->value.scalar = x;
    contents->entries[11]->value.scalar = y;
    contents->entries[12]->value.scalar = z;
}
void CMeshParticleDef::SetStartBox(float x, float y, float z) {
    if (!(x >= 0.0f))
        x = 0.0f;
    if (!(y >= 0.0f))
        y = 0.0f;
    if (!(z >= 0.0f))
        z = 0.0f;
    contents->entries[13]->value.scalar = x;
    contents->entries[15]->value.scalar = y;
    contents->entries[14]->value.scalar = z;
}
void CMeshParticleDef::SetStartDir(float x, float y, float z, float spread) {
    contents->entries[16]->value.scalar = x;
    contents->entries[17]->value.scalar = y;
    contents->entries[18]->value.scalar = z;
    contents->entries[19]->value.scalar = spread;
}
void CMeshParticleDef::SetStartSpeed(float speed, float variance) {
    contents->entries[20]->value.scalar = speed;
    contents->entries[21]->value.scalar = variance;
}
void CMeshParticleDef::SetAccel(float x, float y, float z, float scale) {
    if (!(scale >= 0.0f))
        scale = 0.0f;
    if (!(scale <= 1.0f))
        scale = 1.0f;
    contents->entries[22]->value.scalar = x;
    contents->entries[23]->value.scalar = y;
    contents->entries[24]->value.scalar = z;
    contents->entries[25]->value.scalar = scale;
}
void CMeshParticleDef::SetYaw(float a, float b, float c, float d) {
    contents->entries[26]->value.scalar = a;
    contents->entries[27]->value.scalar = b;
    contents->entries[32]->value.scalar = c;
    contents->entries[33]->value.scalar = d;
}
void CMeshParticleDef::SetPitch(float a, float b, float c, float d) {
    contents->entries[28]->value.scalar = a;
    contents->entries[29]->value.scalar = b;
    contents->entries[34]->value.scalar = c;
    contents->entries[35]->value.scalar = d;
}
void CMeshParticleDef::SetRoll(float a, float b, float c, float d) {
    contents->entries[30]->value.scalar = a;
    contents->entries[31]->value.scalar = b;
    contents->entries[36]->value.scalar = c;
    contents->entries[37]->value.scalar = d;
}
void CMeshParticleDef::SetScale(float start, float startVariance, float end, float endVariance) {
    if (!(start >= 0.0f))
        start = 0.0f;
    if (!(startVariance >= 0.0f))
        startVariance = 0.0f;
    if (!(start - startVariance >= 0.0f))
        startVariance = start;
    if (!(end >= 0.0f))
        end = 0.0f;
    if (!(endVariance >= 0.0f))
        endVariance = 0.0f;
    if (!(end - endVariance >= 0.0f))
        endVariance = end;
    contents->entries[38]->value.scalar = start;
    contents->entries[39]->value.scalar = startVariance;
    contents->entries[40]->value.scalar = end;
    contents->entries[41]->value.scalar = endVariance;
}
void CMeshParticleDef::SetStartColor(float r, float g, float b, float a) {
    if (!(r >= 0.0f))
        r = 0.0f;
    if (!(g >= 0.0f))
        g = 0.0f;
    if (!(b >= 0.0f))
        b = 0.0f;
    if (!(a >= 0.0f))
        a = 0.0f;
    if (!(r <= 255.0f))
        r = 255.0f;
    if (!(g <= 255.0f))
        g = 255.0f;
    if (!(b <= 255.0f))
        b = 255.0f;
    if (!(a <= 255.0f))
        a = 255.0f;
    contents->entries[42]->value.scalar = r;
    contents->entries[43]->value.scalar = g;
    contents->entries[44]->value.scalar = b;
    contents->entries[45]->value.scalar = a;
}
void CMeshParticleDef::SetEndColor(float r, float g, float b, float a) {
    if (!(r >= 0.0f))
        r = 0.0f;
    if (!(g >= 0.0f))
        g = 0.0f;
    if (!(b >= 0.0f))
        b = 0.0f;
    if (!(a >= 0.0f))
        a = 0.0f;
    if (!(r <= 255.0f))
        r = 255.0f;
    if (!(g <= 255.0f))
        g = 255.0f;
    if (!(b <= 255.0f))
        b = 255.0f;
    if (!(a <= 255.0f))
        a = 255.0f;
    contents->entries[46]->value.scalar = r;
    contents->entries[47]->value.scalar = g;
    contents->entries[48]->value.scalar = b;
    contents->entries[49]->value.scalar = a;
}
