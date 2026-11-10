#ifndef GAME_TOILET_H
#define GAME_TOILET_H

// Toilet is the game's file loader. The class, function and global names come from the symbol
// table. Members are inferred from how the reconstructed loader functions use them, and only the
// members those functions reach are named.
enum TLTResourceID { TLTResourceUnknown = 0 };

struct TLTResourceEntry {
    int unknown00;
    int count;  // +0x04: value records, for a table of names.
    int values[0]; // +0x08: `count` records; extent is not established here.
    int value;
};

// A level-contents entry: 12 bytes, a type and a table of value records.
struct LevelFileContentsStruct_ {
    TLTResourceID type;
    int count;              // +0x04: records in the table.
    TLTResourceEntry **data; // +0x08
};

struct RtcArchiveEntry {
    int unknown00;
    char *data; // +0x04, rebased by Dearchive
    int unknown08;
};

struct RtcArchive {
    unsigned char unknown00[0x0c];
    int fileSize;             // +0x0c
    int entryCount;           // +0x10
    int unknown14;
    // Entries follow at +0x18.
    RtcArchiveEntry &Entry(int i) { return ((RtcArchiveEntry *)(this + 1))[i]; }
};

class CRtcFile {
public:
    ~CRtcFile();
    void SetArchiveBuffer(char *buffer, void (*freeFunction)(void *));
    void DeallocateArchiveBuffer();
    void Dearchive();
    int GetFileSize() const;

    RtcArchive *archive;          // +0x00
    int dearchived;               // +0x04
    int needsDearchive;           // +0x08
    void (*freeFunction)(void *); // +0x0c
};

class Toilet {
public:
    int mission;                           // +0x000
    int stage;                             // +0x004
    int asyncState;                        // +0x008
    float asyncPercent;                    // +0x00c
    int loadFileNormalLocked;              // +0x010
    int asyncRequest;                      // +0x014
    char asyncFilename[0x100];             // +0x018
    char *levelBigFile;                    // +0x118
    int levelBigFileSize;                  // +0x11c
    char *bigFile120;                      // +0x120
    int bigFile120Size;                    // +0x124
    char *pauseBigFile;                    // +0x128
    int pauseBigFileSize;                  // +0x12c
    void *compartmentBigFile;              // +0x130
    unsigned char unknown134[0x144 - 0x134];
    char *animationBank;                   // +0x144
    int animationBankSize;                 // +0x148
    void *mvdBigFile;                      // +0x14c
    int mvdBigFileSize;                    // +0x150
    char *relocationBuffer;                // +0x154
    int relocationBufferSize;              // +0x158
    unsigned char unknown15c[0x160 - 0x15c];
    LevelFileContentsStruct_ *contents;    // +0x160
    LevelFileContentsStruct_ *contentsEnd; // +0x164
    int contentCount;                      // +0x168
    unsigned char unknown16c[0x184 - 0x16c];
    CRtcFile rtcFile;                      // +0x184
    int soundBankIndex;                    // +0x194; also added to the state for the progress percentage.
};
extern Toilet g_toilet;

LevelFileContentsStruct_ *TLT_FindNextResourceByType(TLTResourceID, LevelFileContentsStruct_ *);
LevelFileContentsStruct_ *TLT_FindNextResourceByTypeAndVal(TLTResourceID, int, int, LevelFileContentsStruct_ *);
void *TLT_LoadFileFromBigFile(char *, const char *, int *);
void *TLT_LoadFileFromLevelBigFile(const char *, int *);
void GetCompartmentNameFromIndex(int, char *, int);

#endif
