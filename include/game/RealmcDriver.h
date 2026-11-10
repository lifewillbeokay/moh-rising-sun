// AI-assisted view of the memory-card library's GameCube driver (gc_driver.cpp); see docs/Memory.md.
// Class and method names come from the symbol table. Member names are inferred from what the code
// does with them; offsets the code touches are verified.
#ifndef GAME_REALMC_DRIVER_H
#define GAME_REALMC_DRIVER_H
// Declarations and result codes as in the Dolphin SDK's card.h, whose headers do not build under ProDG here.
extern "C" {
typedef void (*CARDCallback)(long chan, long result);
long CARDProbeEx(long chan, long *memSize, long *sectorSize);
long CARDMount(long chan, void *workArea, CARDCallback detachCallback);
long CARDUnmount(long chan);
long CARDCheck(long chan);
long CARDGetSectorSize(long chan, unsigned int *size);
long CARDWrite(void *file, const void *buffer, long length, long offset);
long CARDRead(void *file, void *buffer, long length, long offset);
long CARDClose(void *file);
long CARDFreeBlocks(long chan, long *bytes, long *files);
long CARDCreate(long chan, const char *name, unsigned int size, void *file);
long CARDSetStatus(long chan, long fileNo, void *stat);
long CARDFastDelete(long chan, long fileNo);
long CARDDelete(long chan, const char *name);
long CARDRename(long chan, const char *oldName, const char *newName);
long CARDFormat(long chan);
long CARDGetStatus(long chan, long fileNo, void *stat);
long CARDSetAttributes(long chan, long fileNo, unsigned char attributes);
void CARDInit();
}
enum {
    CARD_RESULT_READY = 0,
    CARD_RESULT_BUSY = -1,
    CARD_RESULT_WRONGDEVICE = -2,
    CARD_RESULT_NOCARD = -3,
    CARD_RESULT_NOFILE = -4,
    CARD_RESULT_IOERROR = -5,
    CARD_RESULT_BROKEN = -6,
    CARD_RESULT_EXIST = -7,
    CARD_RESULT_NOENT = -8,
    CARD_RESULT_INSSPACE = -9,
    CARD_RESULT_NOPERM = -10,
    CARD_RESULT_LIMIT = -11,
    CARD_RESULT_NAMETOOLONG = -12,
    CARD_RESULT_ENCODING = -13,
    CARD_RESULT_CANCELED = -14,
    CARD_RESULT_FATAL_ERROR = -128
};
#define CARDSetBannerFormat(stat, f) ((stat)->bannerFormat = (unsigned char)(((stat)->bannerFormat & ~3) | (f)))
#define CARDSetIconAnim(stat, f) ((stat)->bannerFormat = (unsigned char)(((stat)->bannerFormat & ~4) | (f)))
#define CARDSetIconFormat(stat, n, f) \
    ((stat)->iconFormat = (unsigned short)(((stat)->iconFormat & ~(3 << (2 * (n)))) | ((f) << (2 * (n)))))
#define CARDSetIconSpeed(stat, n, f) \
    ((stat)->iconSpeed = (unsigned short)(((stat)->iconSpeed & ~(3 << (2 * (n)))) | ((f) << (2 * (n)))))
namespace Realmc {
struct CardID {
    unsigned short slot;
    unsigned short type; // 1 selects the NGC sports BIOS file paths.
};
// An icon is a count of images, 1536 or 2048 bytes each by the flag at +0x28.
struct IconInfo {
    int count;
    char *data; // +4
    int speeds[8]; // +8
    int compact; // +0x28
    int animation; // +0x2c
};
struct BannerInfo {
    int wide; // Selects a 6144 or 3584 byte banner.
    char *data; // +4
};
struct FileInfo {
    const char *name;
    int size; // +4
    unsigned char unknown_08[4];
    char *title; // +0xc
    int titleLength; // +0x10
    char *comment; // +0x14
    int commentLength; // +0x18
    IconInfo *icon; // +0x1c
    BannerInfo *banner; // +0x20
};
class CmnFileDescriptor {
  public:
    void Clear();
};
// Layouts as in the Dolphin SDK's card.h: the open file record and the directory status.
struct CARDFileInfoView {
    unsigned char unknown_0[20];
};
struct CARDStatView {
    char fileName[32];
    unsigned int length; // +0x20
    unsigned int time;   // +0x24
    unsigned char gameName[4];
    unsigned char company[2];
    unsigned char bannerFormat; // +0x2e
    unsigned char unknown_2f;
    unsigned int iconAddr; // +0x30
    unsigned short iconFormat; // +0x34
    unsigned short iconSpeed;  // +0x36
    unsigned int commentAddr;  // +0x38
    unsigned char unknown_3c[0x6c - 0x3c];
};
class GcFileDescriptor {
  public:
    void Clear();
    int fileNo;
    unsigned char unknown_04[4];
    CardID card;   // +8
    FileInfo info; // +0xc
    unsigned char unknown_30[0x3c - 0x30];
    int openMode;  // +0x3c
    int inUse;     // +0x40
    void *table;   // +0x44
    CARDFileInfoView cardFile; // +0x48
    CARDStatView stat; // +0x5c
    int position;  // +0xc8; bytes held in the driver's sector buffer.
    int offset;    // +0xcc
    int headerSize; // +0xd0
    int dirty;     // +0xd4
    int status;    // +0xd8; sports BIOS paths keep a mapped result here.
    int needsRead; // +0xdc
    int onCard;    // +0xe0; clear means the sports BIOS path.
};
struct OpenFileDescriptor;
struct FindInfoStruct;
// Only the type names are recovered; the code passes these values through.
enum SeekFrom { SeekFrom_Start = 0, SeekFrom_Current = 1, SeekFrom_End = 2 };
enum FileAttribute { FileAttribute_None = 0 };
struct ConfigSettings;
void *AllocateMemSize(const char *, int, int, int, int);
class DeviceDriver {
  public:
    static void *operator new(unsigned int size) { return AllocateMemSize(0, size, 0, 0, 0); }
    virtual ~DeviceDriver();
};
class GCDriver : public DeviceDriver {
  public:
    GCDriver(const ConfigSettings *);
    virtual ~GCDriver();
    unsigned char unknown_04[4];
    GcFileDescriptor files[3]; // +8
    int sectorSize; // +0x2b4
    void *config; // +0x2b8
    void *workArea; // +0x2bc
    void *buffer; // +0x2c0
    CardID mountedCard; // +0x2c4
    static int MEMCARD_SECTOR_SIZE;
    static int mMounted;
    static int mIsCardPresent;
    int ConvertCardResult(int);
    GcFileDescriptor *FindFreeFileDescriptor();
    GcFileDescriptor *ConvertFileHandleToDescriptor(OpenFileDescriptor *);
    int CardExists(const CardID &);
    int GetFileBlocks(const CardID &, const FileInfo *, unsigned int *);
    int GetSectorSize(const CardID &);
    static void CardRemovalCallback(long, long);
    int Mount(const CardID &);
    int Unmount(const CardID &);
    int GetBannerSize(const FileInfo *);
    int GetIconSize(const FileInfo *);
    int WriteHeaderData(GcFileDescriptor *);
    int WriteFile(OpenFileDescriptor *, void *, int, int *);
    int NGCSportsBioWrite(GcFileDescriptor *, void *, int, int *);
    int NGCSportsBioRead(GcFileDescriptor *, void *, int, int *);
    int NGCSportsBioSeek(GcFileDescriptor *, int, SeekFrom);
    int NGCSportsBioClose(GcFileDescriptor *);
    int FlushWriteBuffer(OpenFileDescriptor *);
    int SetHeaderInfo(GcFileDescriptor *);
    int CloseFile(OpenFileDescriptor *);
    int DeleteFile(const CardID &, const FileInfo *);
    int RenameFile(const CardID &, const FileInfo *, const char *);
    int FormatCard(const CardID &);
    int IsCardPresent();
    int Seek(OpenFileDescriptor *, int, SeekFrom);
    int SetAttributes(CardID, char *, FileAttribute);
    int FindFileNumber(char *, int *);
    int GetOpenFileSize(OpenFileDescriptor *);
    int GetFreeCardSpace(const CardID &, int *, int *);
    int ReadFile(OpenFileDescriptor *, void *, int, int *);
    int NGCSportsBioCreate(GcFileDescriptor *);
};
} // namespace Realmc
#endif
