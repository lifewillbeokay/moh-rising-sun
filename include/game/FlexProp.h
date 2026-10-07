#ifndef GAME_FLEXPROP_H
#define GAME_FLEXPROP_H
#include "CMatrix.h"
#include "StringCRC.h"

// Scoped storage views reconstructed from GR8E69. Member names are descriptive;
// the serialized records have variable tails. See docs/FlexProp.md.
struct FlexPropField {
    int crc;
    int type;
    int offset;
};
// The class header is followed by count twelve-byte field records.
class FlexPropClassFormat {
  public:
    char *name;
    int recordSize;
    FlexPropClassFormat *parent;
    int count;
    FlexPropField fields[0];
};
typedef FlexPropClassFormat FlexPropClassView; // Earlier descriptive view name.
class FlexPropFormat {
  public:
    char *name;
    FlexPropClassFormat *description;
    int recordSize;
    float transform[12];
    int fields[0]; // Variable property payload, converted as four-byte words.
    FlexPropField *GetField(int);
};
struct FlexPropList;
class FlexProp {
  public:
    FlexPropFormat *format;
    void *GetDataPtr(int) const;
    int GetFieldOffset(int) const;
    void *GetData(int) const;
    bool IsFieldValid(int) const;
    bool IsFieldValid(const char *) const;
    int GetInt(int) const;
    int GetInt(const char *) const;
    int GetEnum(int) const;
    int GetEnum(const char *) const;
    float GetFloat(int) const;
    float GetFloat(const char *) const;
    bool GetBool(int) const;
    bool GetBool(const char *) const;
    const char *GetString(int) const;
    const char *GetString(const char *) const;
    FlexPropList *GetList(int) const;
    FlexPropList *GetList(const char *) const;
    int GetFieldType(int) const;
    int GetFieldType(const char *) const;
    void GetPosition(CVector3 &) const;
    void SetPositionZ(float);
    const char *GetClassName() const;
    bool IsA(const char *) const;
};
void DebugMsg(const char *, ...);
#endif
