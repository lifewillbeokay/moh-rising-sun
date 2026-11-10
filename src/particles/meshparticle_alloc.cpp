// AI-assisted reconstruction of the mesh particle pool's allocation; see docs/ParticleRecipes.md.
#include "ParticleRecipe.h"
// Move the first free system to the head of the used list.
static inline MeshParticleSystem *PopFree() {
    MeshParticleSystemList *list = &g_meshParticleSystemList;
    MeshParticleSystem *system = list->free;
    list->free = system->next;
    system->next = list->active;
    list->active = system;
    list->count++;
    return system;
}
// Positive priorities may take any free system; zero requires more than two
// unused slots; negative priorities never allocate.
MeshParticleSystem *MeshParticleSystem::AllocSystem(int priority) {
    MeshParticleSystem *system = 0;
    int available = g_meshParticleSystemList.capacity - g_meshParticleSystemList.count;
    if (priority > 0 && available >= 0) {
        if (g_meshParticleSystemList.free)
            system = PopFree();
    } else if (priority >= 0 && available > 2) {
        if (!g_meshParticleSystemList.free)
            system = 0;
        else
            system = PopFree();
    }
    return system;
}
