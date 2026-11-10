// A scoped view of the glare manager and its resources, for the original's glare fragments. Class and method names come
// from the symbol table; member names and the records' meanings are inferred from the code. CGlare's own layout is not
// modelled beyond the members these fragments read.
#ifndef GAME_GLARE_MANAGER_H
#define GAME_GLARE_MANAGER_H
#include "TARExtension.h"
#include "Toilet.h"
#include "DrawContext.h"
extern "C" void *memset(void *, int, unsigned int);
extern "C" void *DWI_alloc(const char *, int, int);
extern "C" void DWI_free(void *);
float cosf(float);
void *TLT_LoadFromRepository(const char *, int *, bool *);

struct COORD3 {
    float x, y, z;
};
struct CGlareRecord {
    int unknown00, unknown04;
    union {
        int number;
        float real;
        const char *text;
    }; // +0x08
};
// A glare level resource's value records, in order; names are inferred from their use.
struct CGlareRecords {
    CGlareRecord *id;
    CGlareRecord *texture;
    CGlareRecord *unknown08;
    CGlareRecord *fadeStart;
    CGlareRecord *fadeEnd;
    CGlareRecord *coneInner;
    CGlareRecord *coneOuter;
};
struct CGlareLevelResource {
    int unknown00, unknown04;
    CGlareRecords *records; // +0x08
};

class CGlareResource {
  public:
    CGlareResource();
    void Init(LevelFileContentsStruct_ *);
    EAGL::TAR tar;
    CGlareLevelResource *resource; // +0x58
    int GetId() const { return resource->records->id->number; }
};

// Built-in fill: the original clears new vectors with memset.
struct GlarePosition {
    float x, y, z;
    GlarePosition() { memset(this, 0, sizeof(*this)); }
    operator const COORD3 &() const { return *(const COORD3 *)this; }
};
class CGlare {
  public:
    CGlare(CGlareResource &);
    static void InitClass();
    static void ResetClass();
    static void ShutdownClass();
    static CGlare *Create(int, bool);
    void Draw(CDrawContext &);
    bool IsVisible(CDrawContext &) const;
    unsigned char unknown00[0x40];
    GlarePosition position; // +0x40
    unsigned char unknown4c[8];
    CGlareResource *resource; // +0x54
    unsigned char unknown58[0x60 - 0x58];
    CGlare *next; // +0x60
    unsigned char unknown64[0x70 - 0x64];
    void *operator new(unsigned int, void *p) { return p; }
};

struct GlarePool {
    CGlare *storage;
    CGlare *active; // +0x04
    CGlare *free;   // +0x08
    int capacity;   // +0x0c
    int count;      // +0x10
    GlarePool() : storage(0), count(0), active(0), free(0), capacity(0) {}
    void LinkFree() {
        if (capacity > 0) {
            free[capacity - 1].next = 0;
            for (int i = 0; i < capacity - 1; i++)
                free[i].next = &free[i + 1];
        }
    }
    void Init(int size);
    void Reset() {
        active = 0;
        free = storage;
        count = 0;
        LinkFree();
    }
    void Shutdown() {
        DWI_free(storage);
        capacity = 0;
        count = 0;
        storage = 0;
        active = 0;
        free = 0;
    }
    CGlare *Alloc() {
        CGlare *glare = free;
        if (!glare)
            return 0;
        free = glare->next;
        glare->next = active;
        active = glare;
        count++;
        return glare;
    }
};

class CGlareManager {
  public:
    CGlareManager();
    void Init();
    void Reset();
    void Shutdown();
    CGlare *Create(int);
    CGlareResource *Find(int);
    static void ExecuteDraw(CDrawContext &, void **);
    int enabled;
    int unknown04;
    int resourceCount; // +0x08
    CGlareResource *resources; // +0x0c
    GlarePool pool;    // +0x10
};
extern CGlareManager g_manager;

#endif
