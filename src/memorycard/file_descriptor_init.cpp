// A fragment of the memory-card library's file descriptors: GcFileDescriptor's Init clears through its table. Class and
// method names come from the symbol table; member names are inferred from the code.
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
    virtual void Clear(); // Table pointer at +0x44.
};

class GcFileDescriptor : public CmnFileDescriptor {
  public:
    virtual void Clear();
    virtual void Init(const CardID &, const FileInfo &, FileOpenMode);
};

void GcFileDescriptor::Init(const CardID &id, const FileInfo &file, FileOpenMode mode) {
    Clear();
    card = id;
    info = file;
    openMode = mode;
    inUse = 0;
}
}
