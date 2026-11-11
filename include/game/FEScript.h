// FEng scripts and key-frame tracks; class tags from symbols, see docs/Frontend.md. Member names
// are descriptive.
#ifndef GAME_FE_SCRIPT_H
#define GAME_FE_SCRIPT_H
#pragma interface
#include "FEObjectPool.h"
#include "FEEvent.h"

// Sixteen bytes that a track reads as a vector, a colour or similar.
struct FEKeyValue { unsigned long word[4]; };
struct FEVector3 {
    float x, y, z;
    FEVector3() {}
    FEVector3(float px, float py, float pz) {
        z = pz;
        y = py;
        x = px;
    }
    FEVector3 &operator=(const FEVector3 &);
    FEVector3 operator-() const {
        FEVector3 r(-x, -y, -z);
        return r;
    }
    FEVector3 &operator+=(const FEVector3 &o) {
        x += o.x;
        y += o.y;
        z += o.z;
        return *this;
    }
    FEVector3 operator-(const FEVector3 &o) const {
        FEVector3 r;
        r.x = x - o.x;
        r.y = y - o.y;
        r.z = z - o.z;
        return r;
    }
};
class FEColor;

class FEKeyNode : public FEMinNode {
  public:
    long time;        // +0xc
    FEKeyValue value; // +0x10
    static FEMinList NodePool;
    FEKeyNode() {}
    virtual ~FEKeyNode() {}
    void *operator new(unsigned int);
    void operator delete(void *);
};


class FEKeyTrack {
  public:
    unsigned char unknown_0;  // +0x0
    unsigned char valueSize;  // +0x1
    unsigned char unknown_2;  // +0x2
    unsigned char flags;      // +0x3; the value setters clear the top bit.
    long lastKeyTime : 24;    // +0x4
    signed char dataOffset;   // +0x7; compared with FETrackOffsets.
    FEKeyNode baseKey;        // +0x8
    FERefList keys;           // +0x28
    FEKeyTrack() : unknown_0(0), valueSize(0), unknown_2(0), flags(0), lastKeyTime(0), dataOffset(0) {}
    void operator=(FEKeyTrack &);
    FEKeyNode *GetKeyAt(long);
    FEKeyNode *GetDeltaKeyAt(long);
    void InsertKey(FEKeyNode *);
    FEKeyNode *CreateKeyAt(long);
    bool SetKeyValueAt(long, FEVector3 &);
    bool SetKeyValueAt(long, FEColor &);
};

class FEScript : public FEMinNode {
  public:
    unsigned long unknown_c;  // +0xc
    int time;                 // +0x10; SetScript restarts it.
    unsigned long unknown_14; // +0x14
    unsigned long unknown_18; // +0x18
    long trackCount;          // +0x1c
    FEKeyTrack *tracks;       // +0x20
    FEEventList events; // +0x24
    char *name;               // +0x2c
    unsigned long hash;       // +0x30
    static FEMinList NodePool;
    FEScript() { Init(); }
    FEScript(FEScript &, bool);
    virtual ~FEScript();
    void *operator new(unsigned int);
    void operator delete(void *);
    void Init();
    void SetName(const char *);
    void SetTrackCount(long);
    FEKeyTrack *FindTrack(FEKeyTrack_Indices) const;
};
#endif
