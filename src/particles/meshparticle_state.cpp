// AI-assisted reconstruction of the mesh particle system's state accessors; see docs/ParticleRecipes.md.
#include "ParticleRecipe.h"
void MeshParticleSystem::Stop() { flags.active = 0; }
void MeshParticleSystem::Start() {
    if (!flags.dying)
        flags.active = 1;
}
unsigned int MeshParticleSystem::IsEternal() const { return flags.eternal; }
unsigned int MeshParticleSystem::IsActive() const { return flags.active; }
unsigned int MeshParticleSystem::IsDying() const { return flags.dying; }
unsigned int MeshParticleSystem::IsMoving() const { return 0; }
unsigned int MeshParticleSystem::IsRotating() const { return 0; }
unsigned int MeshParticleSystem::IsDestroyable() const { return flags.destroyable; }
void MeshParticleSystem::SetDestroyable(bool destroyable) { flags.destroyable = destroyable; }
CMeshParticleDef *MeshParticleSystem::GetDef() const { return definition; }
MeshParticleSystem *MeshParticleSystem::AsMeshParticleSystem() { return this; }
const MeshParticleSystem *MeshParticleSystem::AsMeshParticleSystem() const { return this; }
