// AI-assisted scoped reconstruction from GR8E69; see docs/Script.md.
#ifndef GAME_SOUND_MANAGER_H
#define GAME_SOUND_MANAGER_H

// Method-only view of the embedded sound schedule.
class SoundSchedule {
  public:
    void ActivateBuss(unsigned int);
    void RemoveSound(unsigned int);
};

// Scoped view of a sound entry: data through +0xd4, then the table pointer.
class SoundEntry {
  public:
    unsigned char unknown_0[0xc];
    int looping; // +0xc; read by BIFunc_IsAliasSoundLooping.
    unsigned char unknown_10[0xd4 - 0x10];
    SoundEntry();
    virtual ~SoundEntry();
};
// Bank-backed entry; the original constructor and destructor are inline.
class SoundEntryBank : public SoundEntry {
  public:
    int field_d8;
    unsigned char field_dc;
    SoundEntryBank() : field_d8(0), field_dc(0) {}
    virtual ~SoundEntryBank() {}
    virtual int GetTypeID() const;
};

// Scoped view of the 0x8700-byte g_SoundMan object (class cSoundMan). Only the
// members the script built-ins touch are named.
class cSoundMan {
  public:
    unsigned char unknown_0[0x837c];
    SoundSchedule schedule; // +0x837c
    unsigned char unknown_837d[0x85f4 - 0x837d];
    int soundZone;          // +0x85f4; set by BIFunc_SetSoundZone.
    void GetSound(SoundEntryBank &, unsigned int);
};
typedef cSoundMan SoundManagerView;
extern cSoundMan g_SoundMan;

// Method-only view of the NPC vocal manager singleton.
class NPCVocalMan {
  public:
    void RemoveVocal(void *, int);
};
extern NPCVocalMan g_npcVocalMan;

class TriggerObject;
class CVector3;
struct BSObject;
int PlaySoundFromTriggerOrObject(TriggerObject *, BSObject *, void *, int, int, CVector3 &);
int PlaySoundFromTriggerOrObject2(TriggerObject *, BSObject *, void *, unsigned int, int, CVector3 &, bool);
#endif
