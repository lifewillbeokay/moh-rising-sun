// A fragment of the original thread.cpp: background jobs run from a single worker thread. Class and
// method names come from the symbol table; the file name from the allocation labels. Member names are
// inferred from this code. The mutex is at the start of the object.
#include "profile.h"
#include <deque>

// The STL queue adapter, whose header this tree does not carry; the symbol table shows its template
// arguments and that its destructor is out of line.
namespace _STL {
template <class T, class Sequence = deque<T> > class queue {
  public:
    bool empty() const { return c.empty(); }
    T &front() { return c.front(); }
    void push(const T &x) { c.push_back(x); }
    void pop() { c.pop_front(); }
    ~queue();

  protected:
    Sequence c;
};
}

extern "C" void *DWI_alloc(const char *, int, int);
extern "C" int DWI_free(void *);
extern "C" void DWI_intfill(void *, int, int);
extern "C" void MUTEX_create(void *);
extern "C" void MUTEX_destroy(void *);
extern "C" void MUTEX_lock(void *);
extern "C" void MUTEX_unlock(void *);
struct OSThread;
extern "C" int THREAD_createparam(OSThread *, void *(*)(void *), void *, void *, unsigned int, int);
extern "C" int THREAD_waitexit(OSThread *, int);
extern "C" void THREAD_destroy(OSThread *);
extern "C" void THREAD_exit();
extern "C" int THREAD_getpriority(OSThread *);
extern "C" int THREAD_setpriority(OSThread *, int);
extern "C" void THREAD_yieldticks(int);

class Thread;
class ThreadJob {
  public:
    void Init(void (*)(ThreadJob &), void (*)(ThreadJob &), void (*)(ThreadJob &), Thread *, void *, int);
    void Reset();
    void (*run)(ThreadJob &);    // +0x00
    void (*begin)(ThreadJob &);  // +0x04
    void (*end)(ThreadJob &);    // +0x08
    Thread *thread;              // +0x0c
    void *data;                  // +0x10
    int id;                      // +0x14
    int priority;                // +0x18
    int aborted;                 // +0x1c
};

class Thread {
  public:
    Thread();
    ~Thread();
    int Init(int, int);
    int DeInit();
    int Add(void (*)(ThreadJob &), void *, int, void (*)(ThreadJob &), void (*)(ThreadJob &));
    void AbortAll();
    int HasJobs();
    static void *ThreadCallback(void *);
    ThreadJob *PopPending();
    ThreadJob *PopFree();
    void PushFree(ThreadJob *);
    void PushPending(ThreadJob *);
    void Reset();
    unsigned char mutex[28];
    unsigned char thread[824 - 28];      // +0x1c; an OSThread and what follows it.
    char *stack;                         // +0x338
    char *stackEnd;                      // +0x33c
    ThreadJob *jobs;                     // +0x340
    ThreadJob *currentJob;               // +0x344
    _STL::queue<ThreadJob *> pending;    // +0x348
    _STL::queue<ThreadJob *> free;       // +0x378
    int jobCount;                        // +0x3a8
    int stackSize;                       // +0x3ac
    int initialized;                     // +0x3b0
    int exiting;                         // +0x3b4
};

void ThreadJob::Init(void (*runFunction)(ThreadJob &), void (*beginFunction)(ThreadJob &), void (*endFunction)(ThreadJob &), Thread *owner, void *userData, int jobPriority) {
    run = runFunction;
    begin = beginFunction;
    end = endFunction;
    thread = owner;
    data = userData;
    priority = jobPriority;
    id = 0;
    aborted = 0;
}

void ThreadJob::Reset() {
    run = 0;
    begin = 0;
    end = 0;
    thread = 0;
    data = 0;
    aborted = 0;
}

Thread::Thread() { Reset(); }

Thread::~Thread() { DeInit(); }

int Thread::Init(int count, int size) {
    if (initialized == 1)
        return 0;
    stackSize = size;
    jobCount = count;
    jobs = (ThreadJob *)DWI_alloc("source/system/thread.cpp:95", count * 32, 1024);
    stack = (char *)DWI_alloc("source/system/thread.cpp:106", stackSize, 1024);
    stackEnd = stack + stackSize;
    DWI_intfill(stack, stackSize, 0xbad4c0de);
    for (int i = 0; i < jobCount; i++) {
        jobs[i].Reset();
        jobs[i].id = i;
        ThreadJob *job = &jobs[i];
        free.push(job);
    }
    initialized = 1;
    THREAD_createparam((OSThread *)thread, ThreadCallback, this, stack, stackSize, -3);
    MUTEX_create(mutex);
    return 1;
}

int Thread::DeInit() {
    if (!initialized)
        return 0;
    if (jobs)
        DWI_free(jobs);
    if (stack)
        DWI_free(stack);
    AbortAll();
    exiting = 1;
    THREAD_waitexit((OSThread *)thread, 0);
    THREAD_destroy((OSThread *)thread);
    MUTEX_destroy(mutex);
    Reset();
    return 1;
}

int Thread::Add(void (*runFunction)(ThreadJob &), void *data, int priority, void (*begin)(ThreadJob &), void (*end)(ThreadJob &)) {
    ThreadJob *job = PopFree();
    if (!job)
        return -1;
    job->Init(runFunction, begin, end, this, data, priority);
    PushPending(job);
    return job->id;
}

void Thread::AbortAll() {
    for (int i = 0; i < jobCount; i++)
        jobs[i].aborted = 1;
}

int Thread::HasJobs() {
    int hasJobs;
    if (pending.empty()) {
        hasJobs = 1;
        if (!currentJob)
            hasJobs = 0;
    } else
        hasJobs = 1;
    return hasJobs;
}

void *Thread::ThreadCallback(void *parameter) {
    Thread *self = (Thread *)parameter;
    while (!self->exiting) {
        self->currentJob = 0;
        if (self->HasJobs()) {
            ThreadJob *job = self->PopPending();
            int priority = THREAD_getpriority((OSThread *)self->thread);
            self->currentJob = job;
            THREAD_setpriority((OSThread *)self->thread, job->priority);
            if (job->begin)
                job->begin(*job);
            if (job->run)
                job->run(*job);
            if (job->end)
                job->end(*job);
            THREAD_setpriority((OSThread *)self->thread, priority);
            self->PushFree(job);
        }
        self->currentJob = 0;
        THREAD_setpriority((OSThread *)self->thread, -3);
        THREAD_yieldticks(10);
    }
    THREAD_exit(); // Does not return.
}

ThreadJob *Thread::PopPending() {
    MUTEX_lock(mutex);
    ThreadJob *job = pending.front();
    pending.pop();
    MUTEX_unlock(mutex);
    return job;
}

ThreadJob *Thread::PopFree() {
    MUTEX_lock(mutex);
    ThreadJob *job = free.front();
    free.pop();
    MUTEX_unlock(mutex);
    return job;
}

void Thread::PushFree(ThreadJob *job) {
    MUTEX_lock(mutex);
    free.push(job);
    MUTEX_unlock(mutex);
}

void Thread::PushPending(ThreadJob *job) {
    MUTEX_lock(mutex);
    pending.push(job);
    MUTEX_unlock(mutex);
}

void Thread::Reset() {
    stack = 0;
    stackEnd = 0;
    jobs = 0;
    currentJob = 0;
    jobCount = 0;
    stackSize = 0;
    initialized = 0;
}
