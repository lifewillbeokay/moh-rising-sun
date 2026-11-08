// AI-assisted scoped reconstruction from GR8E69; see docs/Sound.md and docs/Script.md.
#ifndef GAME_SOUND_MANAGER_H
#define GAME_SOUND_MANAGER_H
#pragma interface
// Keep out-of-line constructors and virtual tables in their original units.

class SoundEntryPool;
struct SNDPLAYOPTS;
// Only the type name is recovered; zero is a descriptive placeholder.
enum SoundCBEventCode { SoundCBEventCode_None = 0 };
class SoundEmitter;

class SoundEntry;
class cSoundBussMan {
  public:
    void RetroFitNewSound(SoundEntry *);
};
// Scoped prefix of the embedded sound schedule.
class SoundSchedule {
  public:
    unsigned char unknown_0[0x228];
    unsigned int nextEventID;
    cSoundBussMan busses;
    // Descriptive inline helper; no historical name is claimed.
    unsigned int AllocateEventID() { return nextEventID++; }
    int Add(SoundEntry *);
    unsigned int AddSounds(const SoundEntryPool &);
    void ActivateBuss(unsigned int);
    void RemoveSound(unsigned int);
};

// Scoped view of a sound entry: data through +0xd4, then the table pointer.
class SoundEntry {
  public:
    unsigned char unknown_0[4];
    int field_04; // Selects distance-based GetOpts volume only when equal to 1.
    int spatial; // +8; enables distance/azimuth processing.
    int looping;
    unsigned char unknown_10[8];
    short pitch;
    unsigned char unknown_1a[2];
    float volume, volumeLimit, attenuation; // +0x1c, +0x20, +0x24; names descriptive.
    unsigned char unknown_28[0x4c - 0x28];
    int field_4c; // Set to 1 for a looping entry with a missing scene emitter.
    SoundEmitter *emitter;
    float distance;
    int azimuth;
    unsigned int emitterStorage[7]; // +0x5c through +0x77; scene emitter uses +0x74.
    unsigned int eventID, field_7c, field_80; // eventID at +0x78.
    int field_84, field_88; // Callback address and event mask; preserved raw-word ABI.
    unsigned char unknown_8c[4];
    int params[3];
    unsigned char unknown_9c[4];
    unsigned short updates; // +0xa0; dirty bits for playback parameters.
    unsigned char unknown_a2[2];
    int handle;
    short patch; // +0xa8
    unsigned char unknown_aa[0xc4 - 0xaa];
    float currentVolume;
    signed char slotMode; // +0xc8; ResolveSlotNum maps values above 1.
    unsigned char unknown_c9[3];
    float field_cc;
    int filterEnabled; // +0xd0; gates the optional playback filter.
    void SetEmitter(const SoundEmitter *);
    void DoNotifyCallback(SoundCBEventCode);
    void Calculate3D();
    int UpDateSoundParams();
    float CalculateVolDistance();
    void GetOpts(SNDPLAYOPTS &, float &);
    void copyData(const SoundEntry &);
    SoundEntry();
    SoundEntry(const SoundEntry &);
    virtual ~SoundEntry();
    // Slot order is shared by the original bank and stream tables. Unused
    // external return types remain provisional where callers do not consume them.
    virtual int GetTypeID() const = 0;
    virtual int Play() = 0;
    virtual void Clone(SoundEntry *) const = 0;
    virtual void UpdatePlayStatus() = 0;
    virtual int IsFinished() = 0;
    virtual void SetFade(float, float, bool) = 0;
    virtual int CheckFade() = 0;
    virtual int Stop() = 0;
    virtual void SetNormSndVol(float) = 0;
    virtual void SetSndPitch(int) = 0;
};
typedef char SoundEntrySizeCheck[sizeof(SoundEntry) == 0xd8 ? 1 : -1];
typedef char SoundSchedulePrefixSizeCheck[sizeof(SoundSchedule) == 0x230 ? 1 : -1];

// Bank-backed entry; the original constructor and destructor are inline.
class SoundEntryBank : public SoundEntry {
  public:
    int field_d8;
    signed char field_dc; // Signed patch-table variation offset.
    SoundEntryBank() : field_d8(0), field_dc(0) {}
    SoundEntryBank(const SoundEntryBank &);
    virtual ~SoundEntryBank();
    void ResolveSlotNum();
    virtual int GetTypeID() const;
    virtual int Play();
    virtual void Clone(SoundEntry *) const;
    virtual void UpdatePlayStatus();
    virtual int IsFinished();
    virtual void SetFade(float, float, bool);
    virtual int CheckFade();
    virtual int Stop();
    virtual void SetNormSndVol(float);
    virtual void SetSndPitch(int);
};

typedef char SoundEntryBankSizeCheck[sizeof(SoundEntryBank) == 0xe0 ? 1 : -1];

// Scoped view of the original 0x8700-byte g_SoundMan object (class cSoundMan).
// SoundSchedule and cSoundBussMan are prefixes, not complete allocation types.
class cSoundMan {
  public:
    unsigned char unknown_0[0x10];
    short *slotMap; // +0x10; patch lookup table, owned externally.
    unsigned char unknown_14[0x80b8 - 0x14];
    float soundVolume; // +0x80b8; descriptive global playback gain.
    unsigned char unknown_80bc[0x837c - 0x80bc];
    SoundSchedule schedule; // +0x837c
    unsigned char unknown_85ac[0x85f4 - 0x85ac]; // Remainder of schedule and following fields.
    int soundZone;          // +0x85f4; set by BIFunc_SetSoundZone.
    void GetSound(SoundEntryBank &, unsigned int);
    void GetSoundEntries(SoundEntryPool &, unsigned int);
    // Descriptive inline accessor; retains the observed base-plus-offset access.
    SoundSchedule &GetSchedule() { return schedule; }
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
