// A fragment of the memory-card library's file descriptors. Class and method names come from the symbol table; member
// names are inferred from the code. Clear and Init are virtual in the original; these views do not need the tables.
extern "C" void *memset(void *, int, unsigned int);

namespace Realmc {
struct CardID {
    unsigned short slot;
    unsigned short type;
};
struct FileInfo {
    unsigned char storage[48];
};
enum FileOpenMode { FileOpenMode_None };

class CmnFileDescriptor {
  public:
    int fileNo;
    unsigned char unknown04[4];
    CardID card;    // +0x08
    FileInfo info;  // +0x0c
    FileOpenMode openMode;   // +0x3c
    int inUse;      // +0x40
    void Clear();
    void Init(const CardID &, const FileInfo &, FileOpenMode);
};

void CmnFileDescriptor::Clear() {
    memset(&info, 0, sizeof(info));
    openMode = (FileOpenMode)1;
    inUse = 0;
}

void CmnFileDescriptor::Init(const CardID &id, const FileInfo &file, FileOpenMode mode) {
    card = id;
    info = file;
    openMode = mode;
    inUse = 0;
}

class GcFileDescriptor : public CmnFileDescriptor {
  public:
    unsigned char table[4];     // +0x44
    unsigned char cardFile[20]; // +0x48
    unsigned char stat[0xc8 - 0x5c];
    int position;   // +0xc8
    int offset;     // +0xcc
    int headerSize; // +0xd0
    int dirty;      // +0xd4
    int status;     // +0xd8
    int needsRead;  // +0xdc
    int onCard;     // +0xe0
    void Clear();
};

void GcFileDescriptor::Clear() {
    memset(&info, 0, sizeof(info));
    int zero = 0;
    int one = 1;
    inUse = zero;
    openMode = (FileOpenMode)one;
    fileNo = zero;
    memset(cardFile, 0, sizeof(cardFile));
    position = zero;
    offset = zero;
    dirty = zero;
    headerSize = zero;
    onCard = zero;
    status = zero;
    needsRead = one;
}
}
