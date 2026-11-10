// AI-assisted reconstruction of the particle system pool's reset; see docs/ParticleRecipes.md.
#include "ParticleRecipe.h"
void CParticleSystem::ResetSystem() {
    ParticleSystemList &list = g_particleSystemList;
    list.active = 0;
    list.free = list.storage;
    list.count = 0;
    if (list.capacity > 0) {
        list.storage[list.capacity - 1].next = 0;
        for (int i = 0; i < list.capacity - 1; i++)
            list.free[i].next = &list.free[i + 1];
    }
}
