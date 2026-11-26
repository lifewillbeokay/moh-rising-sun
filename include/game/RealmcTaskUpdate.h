// AI-assisted, scoped views for gc_interface.cpp's task updates.
// Member and helper names are inferred; these are not complete class declarations.
// Offsets, virtual slots and message payloads are checked against GR8E69.
#ifndef GAME_REALMC_TASK_UPDATE_H
#define GAME_REALMC_TASK_UPDATE_H
#include "RealmcDriver.h"
extern "C" void *memset(void *, int, unsigned int);
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
    virtual void Init(bool); // vptr at +0x10
};
class GcTask : public Task {
public:
    CardID card; // +0x14
    int result, enable;
};
class ArgsTask : public GcTask {
public:
    int args[4]; // Prefix view; individual tasks have different tail sizes.
};
class NameTask : public GcTask {
public:
    char name[36];
    int value; // Find count or file attribute.
};
class TaskTrc : public GcTask {
public:
    int unknown20, mounted, unknown28;
};
class TaskTrcCardInfo : public TaskTrc {
public:
    CardID infoCard;
    int freeBytes, freeFiles;
};
class TaskTrcFile : public TaskTrc {
public:
    FileInfo info;
    unsigned char unknown50[12]; // Remaining FileInfo fields, not used here.
};
class TaskTrcMount : public TaskTrc {
public:
    int unknown2c, options, parentID, unknown38, unknown3c;
    void Start(const CardID &id, int parent) {
        card = id;
        options = 15;
        parentID = parent;
        unknown3c = 0;
        result = 27;
        enable = 0;
        state = 0;
        unknown2c = 0;
        unknown38 = 0;
        manager->StartTask(this);
    }
};
class Message {
public:
    unsigned int id;
    int result;
    unsigned char unknown08[0x34 - 8];
    CardID card;
    int freeBytes, freeFiles;
    unsigned char unknown40[0x54 - 0x40];
    OpenFileDescriptor *file;
    const char *name;
    int readBytes, writtenBytes;
    unsigned char unknown64[0x70 - 0x64];
    int args[8];
    void Set(unsigned int message, int status) {
        id = message;
        result = status;
        args[0] = 0; args[1] = 0; args[2] = 0; args[3] = 0;
        args[4] = 0; args[5] = 0; args[6] = 0; args[7] = 0;
    }
};
class GCMessage : public Message {
public:
    virtual void Clear(); // vptr at +0x90; assignment preserves it.
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
};
class GCInterface {
public:
    static GCDriver *mpDriver;
    static GCMessage mTaskMsg;
    static int mReadFailed, mWriteFailed;
    static ArgsTask mTaskCardExists, mTaskGetCardInfo, mTaskMount, mTaskUnmount,
        mTaskOpen, mTaskClose, mTaskRead, mTaskWrite, mTaskSeek, mTaskFlush;
    static NameTask mTaskDelete, mTaskSetAttribute, mTaskFind;
    static TaskTrc mTaskTrcCardExists;
    static TaskTrcCardInfo mTaskTrcGetCardInfo;
    static TaskTrcFile mTaskTrcListFiles;
    static TaskTrcMount mTaskTrcMount;
    static MessageTimer mMsgTimer;
    static GCMessage *Find(const CardID &, const char *, bool);
    static void UpdateTaskCardExists();
    static void UpdateTaskGetCardInfo();
    static void UpdateTaskMount();
    static void UpdateTaskUnmount();
    static void UpdateTaskOpen();
    static void UpdateTaskClose();
    static void UpdateTaskRead();
    static void UpdateTaskWrite();
    static void UpdateTaskSeek();
    static void UpdateTaskFlush();
    static void UpdateTaskDelete();
    static void UpdateTaskSetAttribute();
    static void UpdateTaskFindFile();
    static void UpdateTaskTrcCardExists();
    static void UpdateTaskTrcGetCardInfo();
    static void UpdateTaskTrcListFiles();
};
}
#endif
