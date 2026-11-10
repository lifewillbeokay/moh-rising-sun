// AI-assisted reconstruction of the particle system's setup from its definition; see docs/ParticleRecipes.md.
#include "ParticleClock.h"
#include "ParticleRecipe.h"
unsigned int MathFunRandomUI32Raw();
void CParticleSystem::Init(CParticleDef *def) {
    definition = def->AsProcDef();
    initialVelocity.Zero();
    acceleration.Zero();
    deactivationTicks = 0.0f;
    flags.unknown_31 = 1;
    flags.active = 1;
    flags.unknown_29 = 0;
    flags.moving = 0;
    flags.rotating = 0;
    flags.destroyable = 1;
    flags.eternal = 0;
    flags.rotating = definition->contents->entries[37]->value.scalar != 0.0f;
    flags.fade = definition->contents->entries[47]->value.scalar != 0.0f &&
                 definition->contents->entries[48]->value.scalar != 0.0f;
    float ticks = CPSManager::GetCurrentTicks();
    startTicks = ticks;
    unknown_154 = ticks;
    unknown_158 = ticks;
    unknown_15c = ticks;
    localToWorld.GetSlot(0);
    particleAcceleration.Set(definition->contents->entries[19]->value.scalar,
                             definition->contents->entries[20]->value.scalar,
                             definition->contents->entries[21]->value.scalar);
    seed = MathFunRandomUI32Raw();
    SetupAlphaFade();
    Reload();
}
static inline void RotateVector(CVector3 &out, const CMatrix &m, const CVector3 &v) {
    CVector3 r;
    r.x = v.x * m.row[0].x + v.y * m.row[1].x + v.z * m.row[2].x;
    r.y = v.x * m.row[0].y + v.y * m.row[1].y + v.z * m.row[2].y;
    r.z = v.x * m.row[0].z + v.y * m.row[1].z + v.z * m.row[2].z;
    out = r;
}
void CParticleSystem::Reload() {
    flags.eternal = definition->contents->entries[5]->value.scalar == 0.0f;
    CVector3 v;
    CheckedMatrix inverse;
    v.Set(definition->contents->entries[19]->value.scalar,
          definition->contents->entries[20]->value.scalar,
          definition->contents->entries[21]->value.scalar);
    inverse.FastInverse(localToWorld);
    RotateVector(particleAcceleration, inverse, v);
}
