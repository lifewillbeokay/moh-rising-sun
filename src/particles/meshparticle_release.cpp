// AI-assisted reconstruction of the mesh particle system pool; see docs/ParticleRecipes.md.
#include "ParticleRecipe.h"
void MeshParticleSystem::ReleaseSystem(MeshParticleSystem *system) {
    MeshParticleSystemList &list = g_meshParticleSystemList;
    if (system == list.active) {
        list.active = system->next;
    } else {
        for (MeshParticleSystem *walk = list.active; walk; walk = walk->next) {
            if (walk->next == system) {
                walk->next = system->next;
                break;
            }
        }
    }
    list.count--;
    system->next = list.free;
    list.free = system;
}
