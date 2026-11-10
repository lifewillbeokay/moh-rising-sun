// AI-assisted reconstruction from GR8E69. The original source/object/GameObjectFactory.cpp, named by its
// allocation label. There is no Frontline counterpart: the record and member names are inferred.
#include <string.h>
extern "C" void *DWI_alloc(const char *, int, int);
extern "C" int DWI_free(void *);
void DebugMsg(const char *, ...);

// Bytes around each object in the pool; defined outside this file.
extern const int BUFSTART_PADSIZE;
extern const int BUFEND_PADSIZE;

class CGameObjectFactory;
class IGameObject;
// The sixteen bytes before each pool object. Serial 0 marks a free record.
struct GameObjectRecord {
    CGameObjectFactory *factory;
    IGameObject *object;
    unsigned int serial;
    int unused;
    bool InUse() const { return serial != 0; }
};
// The record pointer comes before the table pointer; slot names are inferred.
class IGameObject {
  public:
    GameObjectRecord *record;
    virtual ~IGameObject();
    virtual void Recycle();
    void IGO_Destroy();
};

class CGameObjectFactory {
  public:
    CGameObjectFactory();
    int Init(int, int, float);
    void Reset();
    void Shutdown();
    void FreeGameObject(IGameObject *);
    void InitializePool();
    void FreeAllObjects();
    void CheckRecycleThreshold();
    int objectCount;        // +0x00
    int activeCount;        // +0x04
    float recycleThreshold; // +0x08; a fraction of the pool.
    int objectSize;         // +0x0c
    char *pool;             // +0x10
    unsigned int serial;    // +0x14
};

void IGameObject::IGO_Destroy() { record->factory->FreeGameObject(this); }

CGameObjectFactory::CGameObjectFactory()
    : objectCount(0), activeCount(0), recycleThreshold(0.0f), objectSize(0), pool(0), serial(0) {}

int CGameObjectFactory::Init(int size, int count, float threshold) {
    recycleThreshold = threshold;
    objectSize = size;
    int misaligned = size & 15;
    if (misaligned) {
        int rounded = size + 16;
        objectSize = rounded - misaligned;
    }
    int stride = objectSize + BUFSTART_PADSIZE + BUFEND_PADSIZE;
    objectCount = count;
    int bytes = count * stride;
    pool = (char *)DWI_alloc("source/object/GameObjectFactory.cpp:111", bytes, 1024);
    if (!pool) {
        DebugMsg("CGameObjectFactory::Init: failed to allocate pool of %d bytes!\n", bytes);
        return 0;
    }
    InitializePool();
    return 1;
}

void CGameObjectFactory::Reset() {
    FreeAllObjects();
    InitializePool();
}

void CGameObjectFactory::Shutdown() {
    if (pool) {
        FreeAllObjects();
        DWI_free(pool);
        pool = 0;
    }
}

void CGameObjectFactory::FreeGameObject(IGameObject *object) {
    if (object) {
        GameObjectRecord *record = object->record;
        delete object;
        record->serial = 0;
        record->object = 0;
        activeCount--;
    }
}

void CGameObjectFactory::InitializePool() {
    int stride = objectSize + BUFSTART_PADSIZE + BUFEND_PADSIZE;
    for (int i = 0; i < objectCount; i++) {
        GameObjectRecord *record = (GameObjectRecord *)(pool + i * stride);
        memset(record, 0, sizeof(GameObjectRecord));
        record->factory = this;
    }
}

void CGameObjectFactory::FreeAllObjects() {
    int stride = objectSize + BUFSTART_PADSIZE + BUFEND_PADSIZE;
    for (int i = 0; i < objectCount; i++) {
        GameObjectRecord *record = (GameObjectRecord *)(pool + i * stride);
        if (record->InUse())
            FreeGameObject(record->object);
    }
}

void CGameObjectFactory::CheckRecycleThreshold() {
    if (!(recycleThreshold <= 0.0f) && !((float)activeCount / (float)objectCount < recycleThreshold)) {
        GameObjectRecord *oldest = 0;
        int stride = objectSize + BUFSTART_PADSIZE + BUFEND_PADSIZE;
        unsigned int best = serial + 1;
        for (int i = 0; i < objectCount; i++) {
            GameObjectRecord *record = (GameObjectRecord *)(pool + i * stride);
            if (record->serial != 0 && record->serial < best) {
                best = record->serial;
                oldest = record;
            }
        }
        oldest->object->Recycle();
    }
}
