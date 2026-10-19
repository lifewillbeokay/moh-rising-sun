// AI-assisted scoped reconstruction from GR8E69; see docs/TriggerObject.md.
#ifndef GAME_TRIGGER_OBJECT_H
#define GAME_TRIGGER_OBJECT_H
#include "CVector3.h"
#include "FlexProp.h"
#include "BPD.h"

struct FlexPropList;
struct BSObject;

// Bit positions count from the least significant bit; names are descriptive.
struct TriggerObjectFlags {
    unsigned int unknown_31 : 1;
    unsigned int flexFormat : 1;   // Bit 30: properties are a FlexProp, not a legacy record.
    unsigned int unknown_29_1 : 29;
    unsigned int mgUsed : 1;       // Bit 0: MarkMGAsUsed / IsMGUsed.
};

// Scoped view: the flags word, script object at +4 and the properties at +8. Other storage is unknown.
class CMatrix;
class TriggerObject {
  public:
    TriggerObjectFlags flags;
    BSObject *scriptObject; // +4: the trigger's script object (BIFunc_SendEventParent).
    union {
        FlexProp flex;                // When flags.flexFormat is set.
        xyzProperty_Struct *legacy;   // Otherwise.
    } properties;

    int GetFieldOffset(const char *) const;
    void *GetData(int) const;
    int GetEnum(const char *) const;
    int GetInt(const char *) const;
    int GetCRC(const char *) const;
    bool HasField(const char *) const;
    void GetPosition(CVector3 &) const;
    void GetMatrix(CMatrix &) const;
    void GetRotationMatrix(CMatrix &) const;
    void GetForward(CVector3 &) const;
    void SetPositionZ(float);
    int GetLegacyField(int) const;
    const char *GetClassName() const;
    int GetID() const;
    int GetNativeBindingType() const;
    int IsAnimatedLight() const;
    FlexPropList *GetList(const char *) const;
};

int IsPointInTrigger(const CVector3 &, TriggerObject &);
int IsMGUsed(TriggerObject *);
void MarkMGAsUsed(TriggerObject *, bool);

#endif
