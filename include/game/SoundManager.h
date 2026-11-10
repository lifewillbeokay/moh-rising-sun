// AI-assisted scoped reconstruction from GR8E69; see docs/Sound.md and docs/Script.md.
#ifndef GAME_SOUND_MANAGER_H
#define GAME_SOUND_MANAGER_H
#pragma interface
// Keep out-of-line constructors and virtual tables in their original units.

#include "DwiContainers.h"
class SoundEntryPool;
struct SNDPLAYOPTS;
// Only the type name is recovered; zero is a descriptive placeholder.
enum SoundCBEventCode { SoundCBEventCode_None = 0 };
class SoundEmitter;

class SoundEntry;
struct ColumnMap;
class cSoundBuss;
struct BussEvent;
// Member names are inferred. The size is bounded by soundZone, which follows the schedule in cSoundMan.
class cSoundBussMan {
  public:
    unsigned char unknown_0[4];
    int activeCount;                     // +4
    cSoundBuss *busses;                  // +8; singly linked through the buss's next.
    dwi::list<cSoundBuss *> inactive;    // +0xc; at most three retrofits.
    dwi::list<cSoundBuss *> active;      // +0x2c
    static void InitSoundBussFreeList();
    void RetroFitNewSound(SoundEntry *);
    void Update(float);
    cSoundBuss *FindBuss(unsigned int);
    int ActivateBuss(unsigned int);
    void AddInactiveRetroFit(cSoundBuss *);
    void ApplyInactiveBusses(SoundEntry *);
    void AddActiveBusses(cSoundBuss *);
    void ApplyActiveBusses(SoundEntry *);
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
    SoundSchedule();
    void InitBuss();
    void AddBuss(unsigned int, unsigned char *, ColumnMap *, int);
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
    unsigned char unknown_28[4];
    unsigned int bussCRCs[8]; // +0x2c; the busses this entry answers to.
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
    void SetSoundEntry(unsigned char *, ColumnMap *, int);
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
typedef char SoundSchedulePrefixSizeCheck[sizeof(SoundSchedule) == 0x278 ? 1 : -1];

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

// A bank or stream row is reached through the file's column map. Field names are
// inferred from what the loaders do with them.
struct UberFileHeader {
    unsigned char unknown_0[0xc];
    int hasBankTable; // +0xc
    int columnCount; // +0x10
    int rowCount;    // +0x14
    int rowSize;     // +0x18
    int slotCount;   // +0x1c; shorts of slot map between the rows and the bank table.
};
struct ColumnMap {
    unsigned int nameCRC;
    int type;
    int offset;
};
// Only type names are recovered; these enumerators are descriptive placeholders.
enum ESoundUberTask { SoundUberTask_Sound = 0, SoundUberTask_Buss = 1 };
// Row pointer and the CRC its alias column holds. SearchForAlias's table.
struct SoundRowEntry {
    unsigned char *row;
    unsigned int crc;
};
// Copied whole from the file by LoadLevelUbr; VerifyLevelBanks tests the words in order.
struct SoundBankTable {
    int banks[33];
};
class BankMan {
  public:
    unsigned char unknown_0[0x118];
    BankMan();
    void RemoveSoundBanks();
    int GetBankHandle(SoundEntryBank &);
};

// Scoped view of the original 0x8700-byte g_SoundMan object (class cSoundMan).
// Member names are descriptive and inferred from the loaders and stream code.
// The two 8-byte row tables are bounded by their neighbors (4096 and 50 entries).
class cSoundMan {
  public:
    unsigned char *soundData;  // +0
    ColumnMap *columns;        // +4
    unsigned char *rows;       // +8
    int rowSize;               // +0xc
    short *slotMap;            // +0x10; patch lookup table, follows the rows.
    unsigned char unknown_14[0x20 - 0x14];
    SoundBankTable bankTable;  // +0x20
    unsigned char unknown_a4[4];
    int columnCount;           // +0xa8
    int rowCount;              // +0xac
    SoundRowEntry aliases[4096]; // +0xb0
    int hasBankTable;          // +0x80b0
    float field_80b4;
    float soundVolume;         // +0x80b8; descriptive global playback gain.
    UberFileHeader *bussFile;  // +0x80bc
    ColumnMap *bussColumns;    // +0x80c0
    unsigned char *bussRows;   // +0x80c4
    int bussRowSize;           // +0x80c8
    int bussColumnCount;       // +0x80cc
    int bussRowCount;          // +0x80d0
    SoundRowEntry bussAliases[50]; // +0x80d4
    BankMan bankMan;           // +0x8264
    SoundSchedule schedule;    // +0x837c
    int soundZone;             // +0x85f4; set by BIFunc_SetSoundZone.
    unsigned char *streamBuffers[4]; // +0x85f8
    unsigned char streamCount; // +0x8608
    int streams[4];            // +0x860c
    int streamBufferSize;      // +0x861c
    int streamsReady;          // +0x8620
    int levelStreamHandle;     // +0x8624
    void *levelStreamHeader;   // +0x8628
    unsigned char unknown_862c[0x86ac - 0x862c];
    int levelNumber;           // +0x86ac; first and second format arguments in VerifyLevelBanks.
    int levelVariant;          // +0x86b0
    char levelStreamName[0x86f4 - 0x86b4]; // +0x86b4
    float field_86f4;
    int field_86f8;
    int paused;                // +0x86fc; PauseBankSounds records its argument.
    cSoundMan();
    void InitLevel(unsigned char *, unsigned char *, int, int);
    int LoadLevelStreamFile(const char *);
    void RemoveLevelStreamFile();
    int LoadLevelUbr(UberFileHeader *);
    int LoadLevelBus(UberFileHeader *);
    void CreateRowCRCMap();
    void CreateBusRowCRCMap();
    unsigned int GetUbrCRC(unsigned char *, ESoundUberTask);
    unsigned int GetBussCRC(unsigned char *, ESoundUberTask);
    int SearchForAlias(unsigned int);
    char GetMultSettings(unsigned char *, int &, int &, int &);
    int GetSound(SoundEntryBank &, unsigned int);
    int GetSoundEntries(SoundEntryPool &, unsigned int);
    int SelectGroupEntry(int *, int);
    int InitStreams(unsigned char *, int, int);
    void ShutdownStreams();
    void PurgeStreams();
    int GetFreeStream();
    void VerifyLevelBanks();
    void RemoveSoundBanks();
    // Descriptive inline accessor; retains the observed base-plus-offset access.
    SoundSchedule &GetSchedule() { return schedule; }
};
typedef char cSoundManSizeCheck[sizeof(cSoundMan) == 0x8700 ? 1 : -1];
typedef cSoundMan SoundManagerView;
extern cSoundMan g_SoundMan;

// Method-only view of the NPC vocal manager singleton.
class NPCVocalMan {
  public:
    unsigned char unknown_0[0x44];
    int paused; // 0x44; set by Moh3SoundEvent::Pause.
    void RemoveVocal(void *, int);
};
extern NPCVocalMan g_npcVocalMan;

class TriggerObject;
class CVector3;
struct BSObject;
int PlaySoundFromTriggerOrObject(TriggerObject *, BSObject *, void *, int, int, CVector3 &);
int PlaySoundFromTriggerOrObject2(TriggerObject *, BSObject *, void *, unsigned int, int, CVector3 &, bool);
#endif
