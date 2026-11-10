// AI-assisted view of the memory-card library's GameCube interface (gc_interface.cpp); see docs/Memory.md.
// Class, method and static member names come from the symbol table. Only the virtual slots the code
// here reaches are declared. Member names without a symbol are inferred from what the code does with them.
#ifndef GAME_REALMC_INTERFACE_H
#define GAME_REALMC_INTERFACE_H
#include "RealmcDriver.h"
extern "C" {
void *memset(void *, int, unsigned int);
int sprintf(char *, const char *, ...);
int MUTEX_create(void *);
int MUTEX_destroy(void *);
int MUTEX_lock(void *);
int MUTEX_unlock(void *);
int THREAD_create(void *, void (*)(), void *, int, int);
int THREAD_waitexit(void *, int);
void THREAD_yield(int);
}
namespace Realmc {
struct ConfigSettings {
    void *(*memAlloc)(const char *, int, int, int, int);
    void (*memFree)(void *);
    void *(*sizeAlloc)(const char *, int, int, int, int);
    void (*sizeFree)(void *, int);
};
void SetMemVectors(void *(*)(const char *, int, int, int, int), void (*)(void *));
void SetSizeMemVectors(void *(*)(const char *, int, int, int, int), void (*)(void *, int));
void *AllocateMem(const char *, int, int, int, int);
void FreeMem(void *);
void FreeMemSize(void *, int);
enum UserMessage { UserMessage_None = 0 };
enum TaskID { TaskID_None = 0 };
class Locale {
  public:
    void SetTrcFile(const void *);
    unsigned char storage[12];
};
class Message {
  public:
    unsigned int id;
    int unknown_04;
    unsigned char unknown_08[108 - 8];
    Locale *locale; // +108
    int args[8]; // +112
};
class GCMessage : public Message {
  public:
    void *table; // +144
    void Clear();
    // The fields Clear resets, as the code here writes them inline. The name is inferred.
    void Reset() {
        id = 0;
        unknown_04 = 0;
        args[0] = 0;
        args[1] = 0;
        args[2] = 0;
        args[3] = 0;
        args[4] = 0;
        args[5] = 0;
        args[6] = 0;
        args[7] = 0;
    }
};
// mFindResult begins with a message; 36 further bytes are cleared with it.
struct FindResult {
    GCMessage message;
    unsigned char unknown_94[0xb8 - 0x94];
};
class GcTask {
  public:
    void *unknown_00;
    TaskID id; // +4
};
class TaskManager {
  public:
    GcTask *current;
    unsigned char unknown_04[0x50 - 4];
};
// GetMessage adds its argument to elapsed while active is set; the constructor sets limit to 3.
struct MessageTimer {
    int active;
    int elapsed; // +4
    int unknown_08;
    int limit; // +12
};
class Interface {
  public:
    static void *operator new(unsigned int size) { return AllocateMemSize(0, size, 0, 0, 0); }
    static void operator delete(void *p, unsigned int size) { FreeMemSize(p, size); }
    virtual int AddRef();
    virtual int Release();
    static Interface *CreateInstance(const ConfigSettings &);
};
extern Interface *gInterface;
class InterfaceImp : public Interface {
  public:
    InterfaceImp(const ConfigSettings &);
    virtual ~InterfaceImp();
    ConfigSettings settings; // +4
    Locale locale; // +20
    unsigned char mutex[28]; // +32
};
class BlockCalculatorImp {
  public:
    void Init(GCDriver *, unsigned int);
    unsigned char storage[0x18];
};
class GCInterface : public InterfaceImp {
  public:
    GCInterface(const ConfigSettings &);
    virtual int TrcCardExists(const CardID &);
    virtual ~GCInterface();
    virtual int IsBusy();
    virtual void SetTrcMessageFile(const void *);
    virtual void SendMessage(UserMessage, int);
    virtual int GetBlockSize(const CardID &);
    virtual BlockCalculatorImp *GetBlockCalculator();
    virtual void ClearTask();
    int CheckCard(const CardID &);
    static int ConvertUmsgToOption(UserMessage, int, TaskID);
    static void GCMCThread();
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
    static int mReadFailed;
    static int mWriteFailed;
    static int mNoCheckingCardWarning;
    static GCDriver *mpDriver;
    static UserMessage mUserMsg;
    static Locale *mpLocale;
    static void *mStackBuffer;
    static int mStackSize;
    static int mExitThread;
    static GCMessage *mpNewTaskMsg;
    static TaskManager mTaskManager;
    static GCMessage mTaskMsg;
    static FindResult mFindResult;
    static MessageTimer mMsgTimer;
    static unsigned char mMCardThread[792];
    static unsigned char mMutex[28];
    static char mCardName[192];
    static BlockCalculatorImp mBlockCalculator;
};
} // namespace Realmc
#endif
