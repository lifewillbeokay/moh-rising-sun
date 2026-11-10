// A fragment of the original light.cpp: the animated light manager's pools. Names come from the symbol table; the pool's
// members are inferred from the code and share a shape with the other free-list pools.
#include "Light.h"
#include "PlacementNew.h"
#include "SceneObjects.h"
extern "C" void *DWI_alloc(const char *, int, int);

static inline CPropertyAnimLight *First(AnimLightPool &pool) { return pool.active; }

static inline void LinkFree(AnimLightPool &pool) {
    if (pool.capacity > 0) {
        pool.free[pool.capacity - 1].next = 0;
        for (int i = 0; i < pool.capacity - 1; i++)
            pool.free[i].next = &pool.free[i + 1];
    }
}

static inline void InitPool(AnimLightPool &pool, int capacity) {
    pool.capacity = capacity;
    pool.storage = (CPropertyAnimLight *)DWI_alloc("source/system/freepool.h:38", capacity * sizeof(CPropertyAnimLight), 1024);
    pool.active = 0;
    pool.free = pool.storage;
    pool.count = 0;
    LinkFree(pool);
}

static inline void ResetPool(AnimLightPool &pool) {
    pool.active = 0;
    pool.free = pool.storage;
    pool.count = 0;
    LinkFree(pool);
}

static inline void ShutdownPool(AnimLightPool &pool) {
    DWI_free(pool.storage);
    pool.capacity = 0;
    pool.count = 0;
    pool.storage = 0;
    pool.active = 0;
    pool.free = 0;
}

static inline CPropertyAnimLight *AllocPool(AnimLightPool &pool) {
    CPropertyAnimLight *light = pool.free;
    if (!light)
        return 0;
    pool.free = light->next;
    light->next = pool.active;
    pool.active = light;
    pool.count++;
    return light;
}

void CAnimLightManager::InitClass(CScene *newScene) {
    scene = newScene;
    InitPool(propertyLights, 10);
    InitPool(instancedLights, 10);
}

void CAnimLightManager::ShutdownClass() {
    for (CPropertyAnimLight *light = instancedLights.active; light; light = light->next)
        scene->Remove(*light);
    for (CPropertyAnimLight *light = First(propertyLights); light; light = light->next)
        scene->Remove(*light);
    scene = 0;
    ShutdownPool(propertyLights);
    ShutdownPool(instancedLights);
}

void CAnimLightManager::ResetClass() {
    for (CPropertyAnimLight *light = instancedLights.active; light; light = light->next)
        scene->Remove(*light);
    for (CPropertyAnimLight *light = First(propertyLights); light; light = light->next)
        scene->Remove(*light);
    ResetPool(propertyLights);
    ResetPool(instancedLights);
}

CLight *CAnimLightManager::Create(unsigned int crc) {
    for (int i = 0; i < patternCount; i++) {
        if (((MOH_animatedLight_Struct *)patterns)[i].core.field48 == crc) {
            CLight *light = AllocPool(instancedLights);
            if (!light)
                return 0;
            light = new (light) CInstancedAnimLight(&((MOH_animatedLight_Struct *)patterns)[i]);
            scene->Add(*light, 0);
            return light;
        }
    }
    return 0;
}

CLight *CAnimLightManager::Create(MOH_animatedLight_Struct *record) {
    CLight *light = AllocPool(propertyLights);
    if (!light)
        return 0;
    (new (light) CPropertyAnimLight(record))->InitFromProperty();
    scene->Add(*light, 0);
    return light;
}
