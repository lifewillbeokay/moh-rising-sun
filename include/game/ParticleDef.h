#ifndef GAME_PARTICLE_DEF_H
#define GAME_PARTICLE_DEF_H
// Scoped views of the particle definition classes; see docs/ParticleRecipes.md. Names of data members
// and helper types are descriptive. These are prefixes, not complete classes or allocation sizes.
#pragma interface
namespace EAGL {
class TAR;
}
class CStaticMesh;
struct ParticleRecipeValue {
    unsigned int unknown_0, unknown_4;
    union {
        int integer;
        float scalar;
        const char *text;
    } value;
};
struct LevelFileContentsStruct_ {
    unsigned int unknown_0, unknown_4;
    ParticleRecipeValue **entries;
};
class CProcParticleDef;
class CMeshParticleDef;
// Table order and the inline accessors' position in the image come from the original tables at
// 0x802e26c0, 0x802e2660 and 0x802e2600. Crc and the As*Def slots are inline virtuals.
class CParticleDef {
  public:
    unsigned int unknown_0;
    LevelFileContentsStruct_ *contents;
    CParticleDef();
    virtual ~CParticleDef();
    virtual void Init(LevelFileContentsStruct_ *);
    virtual bool IsValid();
    virtual unsigned int Crc() { return 0; }
    virtual CProcParticleDef *AsProcDef() { return 0; }
    virtual const CProcParticleDef *AsProcDef() const { return 0; }
    virtual CMeshParticleDef *AsMeshDef() { return 0; }
    virtual const CMeshParticleDef *AsMeshDef() const { return 0; }
    virtual void *AsEaglDef() { return 0; }
    virtual const void *AsEaglDef() const { return 0; }
};
class CProcParticleDef : public CParticleDef {
  public:
    void *shapeFile;      // +12: inferred, the repository data holding the texture.
    EAGL::TAR *texture;   // +16: inferred, built from that data.
    CProcParticleDef();
    virtual ~CProcParticleDef();
    virtual void Init(LevelFileContentsStruct_ *);
    virtual bool IsValid();
    virtual unsigned int Crc() { return ObjectType(); }
    virtual CProcParticleDef *AsProcDef() { return this; }
    virtual const CProcParticleDef *AsProcDef() const { return this; }
    unsigned int ObjectType() { return contents->entries[0]->value.integer; }
    void SetEmitDelay(int);
    void SetEmitRate(int);
    void SetPartLifetime(float);
    void SetLocMin(float, float, float);
    void SetLocMax(float, float, float);
    void SetVelMin(float, float, float);
    void SetVelMax(float, float, float);
};
class CMeshParticleDef : public CParticleDef {
  public:
    CStaticMesh *mesh;   // +12: inferred, built from the recipe's mesh name.
    CMeshParticleDef();
    virtual ~CMeshParticleDef();
    virtual void Init(LevelFileContentsStruct_ *);
    virtual bool IsValid();
    virtual unsigned int Crc() { return ObjectType(); }
    virtual CMeshParticleDef *AsMeshDef() { return this; }
    virtual const CMeshParticleDef *AsMeshDef() const { return this; }
    unsigned int ObjectType() { return contents->entries[0]->value.integer; }
    void SetSystemLife(float, float);
    void SetParticleLife(float, float);
    void SetEmission(float, float);
    void SetStartPos(float, float, float);
    void SetStartBox(float, float, float);
    void SetStartDir(float, float, float, float);
    void SetStartSpeed(float, float);
    void SetAccel(float, float, float, float);
    void SetYaw(float, float, float, float);
    void SetPitch(float, float, float, float);
    void SetRoll(float, float, float, float);
    void SetScale(float, float, float, float);
    void SetStartColor(float, float, float, float);
    void SetEndColor(float, float, float, float);
};
#endif
