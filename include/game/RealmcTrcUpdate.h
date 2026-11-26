// AI-assisted scoped views for the TRC state machines in gc_interface.cpp.
// Member/helper names are inferred; see docs/research/card-trc.md.
#ifndef GAME_REALMC_TRC_UPDATE_H
#define GAME_REALMC_TRC_UPDATE_H
#include "RealmcDriver.h"
extern "C" int strcmp(const char *, const char *);
namespace Realmc {
class Task;
class TaskManager {
public:
    unsigned char unknown00[0x4c];
    virtual void StartTask(Task *);
    virtual void EndTask(Task *);
};
class Task {
public:
    TaskManager *manager;
    int id, state, step;
    virtual void Init(bool);
    void Next(int value) { state = value; step = 0; }
    void End() { manager->EndTask(this); }
};
class GcTask : public Task {
public:
    CardID card;
    int result, enable;
};
class TaskTrc : public GcTask {
public:
    int unknown20, mounted, opened;
};
// The driver header exposes the first 36 bytes; task records copy all 48.
struct FullFileInfo : FileInfo {
    int text24, text28, unknown2c;
    int Label() const { return *(const int *)unknown_08; }
};
class TaskTrcCardInfo : public TaskTrc {
public:
    CardID infoCard;
    int freeBytes, freeFiles;
};
class TaskTrcFile : public TaskTrc {
public:
    FullFileInfo info;
};
class TaskTrcStart : public TaskTrcFile {
public:
    unsigned int requiredBlocks;
    int unknown60, scanBoth;
    const char *temporaryName;
    unsigned int requiredFiles, temporaryBlocks;
    int useTemporary, finalResult;
};
class TaskTrcLoad : public TaskTrcFile {
public:
    int unknown5c, driverResult;
    OpenFileDescriptor *Handle() const { return (OpenFileDescriptor *)unknown20; }
};
class TaskTrcSave : public TaskTrcFile {
public:
    int unknown5c, unknown60;
    OpenFileDescriptor *handle;
    int unknown68;
    const char *originalName;
    int driverResult, choice, fileCount, unknown7c;
};
class TaskTrcDelete : public TaskTrcFile {
public:
    int unknown5c, driverResult, choice;
};
class TaskTrcMount : public TaskTrc {
public:
    int choice, formatResult, parentID, keepMounted, removed;
    void Start(const CardID &value, int parent) {
        card = value; formatResult = 15; parentID = parent; removed = 0;
        result = 27; enable = 0; state = 0; choice = 0; keepMounted = 0;
        manager->StartTask(this);
    }
};
class TaskTrcMountOperation : public TaskTrc {
public:
    int choice, parentID, driverResult, foundCard, wrongDevice, success, ioError;
    void Start(const CardID &value, int parent) {
        result = 27; card = value; parentID = parent; driverResult = 16;
        ioError = 0; enable = 0; state = 0; choice = 0; mounted = 0;
        foundCard = 0; wrongDevice = 0; success = 0;
        manager->StartTask(this);
    }
};
struct MessageDetail {
    int message;
    unsigned char unknown04[0x2c - 4];
    CardID card;
    int freeBytes, freeFiles;
    const char *foundName;
    unsigned int foundSize;
    unsigned char unknown40[0x4c - 0x40];
    OpenFileDescriptor *file;
    const char *fileName;
    int readBytes, writtenBytes;
    unsigned char unknown5c[8];
};
class Message {
public:
    unsigned int id;
    int result;
    MessageDetail details;
    void *locale;
    int args[8];
    void Set(unsigned int value, int status) {
        id = value; result = status;
        args[0] = 0; args[1] = 0; args[2] = 0; args[3] = 0;
        args[4] = 0; args[5] = 0; args[6] = 0; args[7] = 0;
    }
    void Prompt(int message, int count) {
        Set(24, 0); details.message = message; args[0] = count;
    }
};
class GCMessage : public Message {
public:
    virtual void Clear();
    int LcGetSlotString(int);
    void LC_SetOptions(int, int, char *);
};
class TaskTrcCheckSpace : public TaskTrcFile {
public:
    int parentID, choice;
    GCMessage *found;
    unsigned int freeBlocks, reclaimedBlocks, neededBlocks;
    unsigned int freeFiles, reclaimedFiles, neededFiles;
    unsigned int totalBlocks, totalFiles, temporaryBlocks, temporaryFiles;
    int exists;
    void Start(const CardID &value, const FullFileInfo &file, int parent, bool notify) {
        card = value; result = 27; enable = notify; state = 0;
        info = file; parentID = parent; choice = 0; found = 0;
        freeBlocks = 0; reclaimedBlocks = 0; neededBlocks = 0;
        freeFiles = 0; reclaimedFiles = 0; neededFiles = 0;
        totalBlocks = 0; totalFiles = 0; temporaryBlocks = 0; temporaryFiles = 0;
        exists = 0;
        manager->StartTask(this);
    }
};
struct MessageTimer {
    int active;
    unsigned int elapsed, duration;
    int seconds;
    bool Ready() {
        if (!active) return true;
        if (duration < elapsed) { active = 0; return true; }
        return false;
    }
    void Start() { active = 1; elapsed = 0; duration = seconds * 1000; }
};
enum UserMessage { UserMessage_None = 0 };
enum TaskID { TaskID_None = 0 };
class GCInterface {
public:
    static GCDriver *mpDriver;
    static GCMessage mTaskMsg;
    static UserMessage mUserMsg;
    static int mReadFailed, mWriteFailed;
    static TaskTrcStart mTaskTrcStartGame;
    static TaskTrc mTaskTrcCardExists;
    static TaskTrcCardInfo mTaskTrcGetCardInfo;
    static TaskTrcLoad mTaskTrcLoadFile;
    static TaskTrcSave mTaskTrcSaveFile;
    static TaskTrcDelete mTaskTrcDeleteFile;
    static TaskTrcFile mTaskTrcListFiles;
    static TaskTrcMount mTaskTrcMount;
    static TaskTrcMountOperation mTaskTrcMountOperation;
    static TaskTrcCheckSpace mTaskTrcCheckSpace;
    static MessageTimer mMsgTimer;
    static GCMessage *Find(const CardID &, const char *, bool);
    static int CheckCard(const CardID &);
    static int ConvertUmsgToOption(UserMessage, int, TaskID);
    static int Choice(int options, int parent) {
        return ConvertUmsgToOption(mUserMsg, options, (TaskID)parent);
    }
    static void UpdateTaskTrcStartGame();
    static void UpdateTaskTrcCardExists();
    static void UpdateTaskTrcGetCardInfo();
    static void UpdateTaskTrcLoadFile();
    static void UpdateTaskTrcSaveFile();
    static void UpdateTaskTrcDeleteFile();
    static void UpdateTaskTrcListFiles();
    static void UpdateTaskTrcMountCard();
    static void UpdateTaskTrcMountCardOperation();
    static void UpdateTaskTrcCheckSpace();
};
// Argument signatures used by the localization calls. The helpers' names are inferred.
inline void Options1(GCMessage &message, int options, int task) { message.LC_SetOptions(options, task, "s"); }
inline void Options2(GCMessage &message, int options, int task) { message.LC_SetOptions(options, task, "ss"); }
inline void Options3(GCMessage &message, int options, int task) { message.LC_SetOptions(options, task, "sss"); }
inline void Options4(GCMessage &message, int options, int task) { message.LC_SetOptions(options, task, "ssss"); }
inline void OptionsSpace(GCMessage &message, int options, int task) { message.LC_SetOptions(options, task, "ssdd"); }
inline void OptionsTemporary(GCMessage &message, int options, int task) { message.LC_SetOptions(options, task, "ssddsdd"); }
}
#endif
