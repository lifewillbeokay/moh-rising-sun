// AI-assisted reconstruction of the mesh particle pool's setup; see docs/ParticleRecipes.md.
#include "ParticleRecipe.h"
extern "C" void *DWI_alloc(const char *, int, int);
enum TLTResourceID {};
LevelFileContentsStruct_ *TLT_FindNextResourceByType(TLTResourceID, LevelFileContentsStruct_ *);
extern int g_numActiveMeshParticles;
// The original pool's allocation is tagged with this line of its header.
static inline void InitPool(MeshParticleSystemList &list, int capacity) {
    list.capacity = capacity;
    list.storage = (MeshParticleSystem *)DWI_alloc("source/system/freepool.h:38", capacity * sizeof(MeshParticleSystem), 1024);
    list.active = 0;
    list.free = list.storage;
    list.count = 0;
    if (list.capacity > 0) {
        list.storage[list.capacity - 1].next = 0;
        for (int i = 0; i < list.capacity - 1; i++)
            list.free[i].next = &list.free[i + 1];
    }
}
void MeshParticleSystem::Init() {
    int capacity = 0;
    LevelFileContentsStruct_ *resource = TLT_FindNextResourceByType((TLTResourceID)27, 0);
    ParticleRecipeValue *setting = resource->entries[110];
    if ((int)setting->unknown_4 > 0)
        capacity = setting->value.integer;
    if (capacity < 10)
        capacity = 10;
    InitPool(g_meshParticleSystemList, capacity);
    g_numActiveMeshParticles = 0;
}
