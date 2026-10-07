// AI-assisted reconstruction from GR8E69; see docs/Bullets.md.
#include "Bullet.h"
void CThrownBullet::SetDamage(float damage) { properties->damage = damage; }
void CThrownBullet::SetBlastRadius(float radius) { properties->blastRadius = radius; }
void CThrownBullet::SetExplosionParticleSystem(unsigned long id, EXPLOSION_PARTICLE_TYPE type) { explosionParticleSystems[type] = id; }
