// A scoped view of the memory-card library's public interface and tasks, for the public.cpp fragments. Class, method
// and static member names come from the symbol table; member names and ArgsTask, NameTask and the concrete TaskTrc shapes
// are inferred from these calls. The GCInterface methods are virtual in the original; their table is not modelled here,
// and static task members all have their own concrete classes in the original.
#ifndef GAME_REALMC_PUBLIC_H
#define GAME_REALMC_PUBLIC_H
extern "C" {
int MUTEX_lock(void *);
int MUTEX_unlock(void *);
char *strncpy(char *, const char *, unsigned int);
void *memset(void *, int, unsigned int);
}

namespace Realmc {
struct CardID {
    unsigned short slot;
    unsigned short type;
};
struct FileInfo {
    int name; // +0x00; copied on its own by TrcSaveFile.
    unsigned char unknown04[48 - 4];
};
struct StartGameInfo {
    unsigned char unknown00[0x34];
    CardID card; // +0x34
    unsigned char unknown38[76 - 0x38];
};
struct OpenFileDescriptor;
enum FileOpenMode { FileOpenMode_None };
enum SeekFrom { SeekFrom_None };
enum FileAttribute { FileAttribute_None };

class Task;
class TaskManager {
  public:
    unsigned char unknown00[0x4c];
    virtual void StartTask(Task *); // Table pointer at +0x4c.
};
class Task {
  public:
    TaskManager *manager;
    unsigned char unknown04[4];
    int status; // +0x08
    int unknown0c;
    virtual void Init(bool); // Table pointer at +0x10.
};
class GcTask : public Task {
  public:
    unsigned short slot; // +0x14
    int kind;            // +0x18
    int enable;          // +0x1c
    void Init(bool value) {
        slot = 0;
        kind = 27;
        enable = value;
        status = 0;
    }
};
// Each task keeps its arguments after the common fields. Members here are the shape of these calls; the name is inferred.
class ArgsTask : public GcTask {
  public:
    int args[4]; // +0x20
    CardID &Card() { return *(CardID *)&slot; }
};
// A task whose first argument is a file name; the name is inferred.
class NameTask : public GcTask {
  public:
    char name[36];  // +0x20
    int result;     // +0x44; FindFile's clear, SetFileAttribute's attribute.
    CardID &Card() { return *(CardID *)&slot; }
};
class TaskTrc : public GcTask {
  public:
    int unknown20, unknown24, unknown28; // +0x20
    virtual void Clear();
    void Init(bool value) {
        Clear();
        GcTask::Init(value);
    }
    void Start() {
        Clear();
        TaskTrc::Init(true);
    }
    CardID &Card() { return *(CardID *)&slot; }
};
class TaskTrcCardInfo : public TaskTrc {
  public:
    CardID card; // +0x2c
    unsigned char unknown30[8];
};
class TaskTrcStart : public TaskTrc {
  public:
    StartGameInfo info; // +0x2c
    int unknown78;
};
class TaskTrcFile : public TaskTrc {
  public:
    FileInfo info; // +0x2c
    int unknown5c[4];
    int saveName; // +0x6c
    int unknown70[2];
    int fileCount; // +0x78
};

class GCInterface {
  public:
    void TrcStartGame(const StartGameInfo &);
    void TrcCardExists(const CardID &);
    void TrcGetCardInfo(const CardID &);
    void TrcLoadFile(const CardID &, const FileInfo &);
    void TrcSaveFile(const CardID &, const FileInfo &);
    void TrcListFiles(const CardID &, const FileInfo &);
    void TrcDeleteFile(const CardID &, const FileInfo &);
    void CardExists(const CardID &);
    void GetCardInfo(const CardID &);
    void Mount(const CardID &);
    void Unmount(const CardID &);
    void OpenFile(const CardID &, const FileInfo &, FileOpenMode);
    void CloseFile(OpenFileDescriptor *);
    void Read(OpenFileDescriptor *, void *, int);
    void Write(OpenFileDescriptor *, void *, int);
    void Seek(OpenFileDescriptor *, int, SeekFrom);
    void Flush(OpenFileDescriptor *);
    void DeleteFile(const CardID &, const char *, const char *);
    void SetFileAttribute(const CardID &, const char *, const char *, FileAttribute);
    void FindFile(const CardID &, const char *, const char *);
    static unsigned char mMutex[28];
    static TaskTrcStart mTaskTrcStartGame;
    static TaskTrc mTaskTrcCardExists;
    static TaskTrcCardInfo mTaskTrcGetCardInfo;
    static TaskTrcFile mTaskTrcLoadFile;
    static TaskTrcFile mTaskTrcSaveFile;
    static TaskTrcFile mTaskTrcListFiles;
    static TaskTrcFile mTaskTrcDeleteFile;
    static ArgsTask mTaskCardExists;
    static ArgsTask mTaskGetCardInfo;
    static ArgsTask mTaskMount;
    static ArgsTask mTaskUnmount;
    static ArgsTask mTaskOpen;
    static ArgsTask mTaskClose;
    static ArgsTask mTaskRead;
    static ArgsTask mTaskWrite;
    static ArgsTask mTaskSeek;
    static ArgsTask mTaskFlush;
    static NameTask mTaskDelete;
    static NameTask mTaskSetAttribute;
    static NameTask mTaskFind;
};
}
#endif
