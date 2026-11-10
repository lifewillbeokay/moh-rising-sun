// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic reference.
// Part of the original bulletfactory.cpp: the instantiations of its helper template. Each pooled bullet's
// IGameObject base is at +0x38; the headers do not model that base yet, so the conversion is written here.
#include "Bullet.h"
#include <string.h>
void DebugMsg(const char *, ...);
extern const int BUFSTART_PADSIZE;
extern const int BUFEND_PADSIZE;

class IGameObject;
class CGameObjectFactory;
struct GameObjectRecord {
    CGameObjectFactory *factory;
    IGameObject *object;
    unsigned int serial;
    int unused;
    bool InUse() const { return serial != 0; }
};
class IGameObject {
  public:
    GameObjectRecord *record;
    void IGO_Destroy();
};
class CGameObjectFactory {
  public:
    CGameObjectFactory();
    int Init(int, int, float);
    void Reset();
    void Shutdown();
    void CheckRecycleThreshold();
    template <class T> IGameObject *AllocateGameObject(T *&object) {
        CheckRecycleThreshold();
        int stride = objectSize + BUFSTART_PADSIZE + BUFEND_PADSIZE;
        if (activeCount == objectCount) {
            DebugMsg("Warning: CGameObjectFactory::AllocateGameObject: max number of objects (%d) are already allocated.\n", objectCount);
            return 0;
        }
        for (int i = 0; i < objectCount; i++) {
            GameObjectRecord *record = (GameObjectRecord *)(pool + i * stride);
            if (!record->InUse()) {
                activeCount++;
                serial++;
                record->serial = serial;
                record->object = 0;
                char *memory = (char *)record + BUFSTART_PADSIZE;
                memset(memory, 0, objectSize);
                object = new (memory) T;
                IGameObject *base = object ? (IGameObject *)((char *)object + 0x38) : 0;
                unsigned int current = serial;
                record->object = base;
                record->serial = current;
                ((IGameObject *)((char *)object + 0x38))->record = record;
                return object ? (IGameObject *)((char *)object + 0x38) : 0;
            }
        }
        return 0;
    }
    int objectCount;
    int activeCount;
    float recycleThreshold;
    int objectSize;
    char *pool;
    unsigned int serial;
};

// The pooled objects' full sizes. The bullet views stop short of the objects' ends.
inline int PooledSize(CProjectileBullet *) { return 448; }
inline int PooledSize(CThrownBullet *) { return 640; }

template <class T, class P, class E> class CBulletFactoryHelper {
  public:
    P *properties;              // +0x00
    unsigned char unknown_4[12];
    CGameObjectFactory factory; // +0x10
    unsigned char unknown_28[8];
    T *first;                   // +0x30
    int initialized;            // +0x34
    unsigned char unknown_38[64 - 0x38];
    CBulletFactoryHelper();
    void Init(P *);
    void Reset();
    void Shutdown();
    T *CreateBullet(E, const CVector3 &, const CVector3 &, float, EClsnId, float, ISceneNode *);
    void DestroyBullet(T *);
};

template <class T, class P, class E>
CBulletFactoryHelper<T, P, E>::CBulletFactoryHelper() {
    properties = 0;
    first = 0;
    initialized = 0;
}

template <class T, class P, class E> void CBulletFactoryHelper<T, P, E>::Init(P *table) {
    properties = table;
    first = 0;
    factory.Init(PooledSize((T *)0), 64, 0.0f);
    initialized = 1;
}

template <class T, class P, class E> void CBulletFactoryHelper<T, P, E>::Reset() {
    first = 0;
    factory.Reset();
}

template <class T, class P, class E> void CBulletFactoryHelper<T, P, E>::Shutdown() {
    first = 0;
    factory.Shutdown();
    initialized = 0;
}

template <class T, class P, class E>
T *CBulletFactoryHelper<T, P, E>::CreateBullet(E type, const CVector3 &position, const CVector3 &direction, float speed,
                                               EClsnId id, float damage, ISceneNode *owner) {
    T *bullet = 0;
    if (!factory.AllocateGameObject(bullet))
        return 0;
    bullet->Init(type, &properties[type], position, direction, speed, id, damage, owner, false);
    bullet->nextBullet = first;
    first = bullet;
    return bullet;
}

template <class T, class P, class E> void CBulletFactoryHelper<T, P, E>::DestroyBullet(T *bullet) {
    T **link = &first;
    T *node = first;
    while (node != bullet) {
        link = &node->nextBullet;
        node = node->nextBullet;
    }
    *link = bullet->nextBullet;
    bullet->Shutdown();
    ((IGameObject *)((char *)bullet + 0x38))->IGO_Destroy();
}

typedef CBulletFactoryHelper<CProjectileBullet, ProjectileBulletProperties_struct, EProjectileBulletTypes> ProjectileHelper;
typedef CBulletFactoryHelper<CThrownBullet, ThrownBulletProperties_struct, EThrownBulletTypes> ThrownHelper;
template ProjectileHelper::CBulletFactoryHelper();
template ThrownHelper::CBulletFactoryHelper();
template void ProjectileHelper::Init(ProjectileBulletProperties_struct *);
template void ThrownHelper::Init(ThrownBulletProperties_struct *);
template void ProjectileHelper::Reset();
template void ThrownHelper::Reset();
template void ProjectileHelper::Shutdown();
template void ThrownHelper::Shutdown();
template CProjectileBullet *ProjectileHelper::CreateBullet(EProjectileBulletTypes, const CVector3 &, const CVector3 &, float, EClsnId, float, ISceneNode *);
template CThrownBullet *ThrownHelper::CreateBullet(EThrownBulletTypes, const CVector3 &, const CVector3 &, float, EClsnId, float, ISceneNode *);
template void ProjectileHelper::DestroyBullet(CProjectileBullet *);
template void ThrownHelper::DestroyBullet(CThrownBullet *);
