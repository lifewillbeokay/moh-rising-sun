#ifndef CHARACTERSHADOW_H
#define CHARACTERSHADOW_H
// A scoped view of a character's projected shadow. Method names come from the original symbols;
// member names follow what the code does with them, and the table pointer is at +36.
class CDrawContext;
class CAnimObject;
class CProjector;

class CCharacterShadow {
public:
    virtual ~CCharacterShadow();
    virtual void Draw(CDrawContext &);
    virtual unsigned int IsDrawEnabled() const;
    CCharacterShadow();
    static void InitClass();
    void InitData();
    void Release();
    void AttachProjector();
    void InitProjector();
    void SetShadowCaster(CAnimObject *);
    void Update(float);

    int enabled;
    CAnimObject *caster;   // +4
    int hipJoints[2];      // +8: left and right joint indices.
    void *field10;
    float scale;
    float field18;
    CProjector *projector;
    void *field20;
};
#endif
