// REAL threads (threads.cpp, the whole file): the priority mappings between
// REAL's scale (-3 to 3) and the OS's 0 to 31, thread creation and destruction,
// sleeping on a signal until an alarm fires, and queries of a thread given as a
// handle where 0 is the main thread and -1 the current one. The record keeping
// the alarm with its signal, the OS structures and parameter roles are
// inferred; g_thMain is declared extern as original context.
struct OSAlarm {
    int data[10];
};

struct OSContext;
struct OSThread;

struct OSMessageQueue {
    int data[8];
};

struct SIGNAL {
    int field0;
    OSMessageQueue queue;
    void* message[1];
};

struct THREADALARM {
    OSAlarm alarm;
    SIGNAL signal;
};

extern "C" {
OSThread* OSGetCurrentThread(void);
int OSCreateThread(OSThread*, void* (*)(void*), void*, void*, unsigned int, int, unsigned short);
int OSResumeThread(OSThread*);
void OSCancelThread(OSThread*);
void OSExitThread(void*);
int OSIsThreadTerminated(OSThread*);
long OSGetThreadPriority(OSThread*);
int OSSetThreadPriority(OSThread*, long);
void SIGNAL_set(SIGNAL*);
void SIGNAL_create(SIGNAL*);
void SIGNAL_wait(SIGNAL*);
void OSYieldThread(void);
void OSCreateAlarm(OSAlarm*);
void OSSetAlarm(OSAlarm*, long long, void (*)(OSAlarm*, OSContext*));
void OSCancelAlarm(OSAlarm*);
int THREAD_testexit(OSThread*);
void THREAD_yield(int);
int THREAD_createparam(OSThread*, void* (*)(void*), void*, void*, unsigned int, int);
}
void InitAlarm();

extern OSThread* g_thMain;

static int ThreadRealPriority2GCPriority(int priority) {
    switch (priority) {
    case 3:
        return 0;
    case 2:
        return 5;
    case 1:
        return 10;
    case 0:
        return 16;
    case -1:
        return 20;
    case -2:
        return 25;
    case -3:
        return 31;
    }
    return 16;
}

static int ThreadGCPriority2RealPriority(long priority) {
    if (priority == 0)
        return 3;
    if (priority <= 5)
        return 2;
    if (priority <= 10)
        return 1;
    if (priority <= 16)
        return 0;
    if (priority <= 20)
        return -1;
    if (priority <= 25)
        return -2;
    return -3;
}

extern "C" void THREAD_init(void) {
    InitAlarm();
    g_thMain = OSGetCurrentThread();
}

extern "C" void THREAD_restore(void) {
}

extern "C" int THREAD_create(OSThread* thread, void* (*fn)(void*), void* stack, unsigned int size, int priority) {
    return THREAD_createparam(thread, fn, 0, stack, size, priority);
}

extern "C" int THREAD_createparam(OSThread* thread, void* (*fn)(void*), void* param, void* stack, unsigned int size, int priority) {
    int ok = 1;

    if (!OSCreateThread(thread, fn, param, (char*)stack + size, size, ThreadRealPriority2GCPriority(priority), 1))
        ok = 0;
    OSResumeThread(thread);
    return ok;
}

extern "C" void THREAD_destroy(OSThread* thread) {
    if (!THREAD_testexit(thread)) {
        OSThread* t = thread == (OSThread*)-1 ? OSGetCurrentThread() : thread;

        if (t)
            OSCancelThread(t);
    }
}

static void AlarmHandler(OSAlarm* alarm, OSContext*) {
    SIGNAL_set(&((THREADALARM*)alarm)->signal);
}

static void THREAD_yield64(long long ticks) {
    THREADALARM sleep;

    if (!ticks) {
        OSYieldThread();
        return;
    }
    SIGNAL_create(&sleep.signal);
    OSCreateAlarm(&sleep.alarm);
    OSSetAlarm(&sleep.alarm, ticks, AlarmHandler);
    SIGNAL_wait(&sleep.signal);
    OSCancelAlarm(&sleep.alarm);
}

/* inferred view of the OS globals at 0x80000000: the original's symbol table
   has __OSBusClock at +0xF8, and an OS timer tick is four bus cycles. */
struct OSBOOTVIEW {
    char unknown00[248];
    unsigned int busclock;
};


extern "C" void THREAD_yield(int ms) {
    THREAD_yield64((long long)ms * (((OSBOOTVIEW*)0x80000000)->busclock / 4 / 1000));
}

extern "C" void THREAD_yieldticks(int ticks) {
    THREAD_yield64(ticks);
}

/* inferred helper: 0 is the main thread, -1 the current one */
static inline OSThread* resolvethread(OSThread* thread) {
    if (!thread)
        return g_thMain;
    if (thread == (OSThread*)-1)
        return OSGetCurrentThread();
    return thread;
}

extern "C" int THREAD_iscurrent(OSThread* thread) {
    return OSGetCurrentThread() == resolvethread(thread);
}

extern "C" int THREAD_getpriority(OSThread* thread) {
    return ThreadGCPriority2RealPriority(OSGetThreadPriority(resolvethread(thread)));
}

extern "C" int THREAD_setpriority(OSThread* thread, int priority) {
    OSThread* t = resolvethread(thread);

    OSSetThreadPriority(t, ThreadRealPriority2GCPriority(priority));
    return 1;
}

extern "C" int THREAD_testexit(OSThread* thread) {
    return OSIsThreadTerminated(resolvethread(thread)) != 0;
}

// Thread::DeInit passes a second argument of 0; this body does not read it.
extern "C" int THREAD_waitexit(OSThread* thread, int) {
    while (!THREAD_testexit(thread))
        THREAD_yield(10);
    return 1;
}

extern "C" void THREAD_exit(void) {
    OSExitThread(0);
}
