// AI-assisted reconstruction from GR8E69; see docs/Bullets.md.
#include "Bullet.h"
void CBullet::GetPosition(CVector3 &v) const { v = localToWorld.GetPos(); }
void CBullet::GetRightward(CVector3 &v) const { v = localToWorld.GetRight(); }
void CBullet::GetForward(CVector3 &v) const { v = localToWorld.GetFront(); }
void CBullet::GetUp(CVector3 &v) const { v = localToWorld.GetUp(); }
void CBullet::GetTMLocalToWorld(CMatrix &out) const { out = localToWorld; }
CBullet *CBullet::AsBullet() { return this; }
const CBullet *CBullet::AsBullet() const { return this; }
float CBullet::GetDamage() const { return 0.0f; }
int CBullet::GetScriptBulletType() const { return -1; }
ISceneNode *CBullet::GetFiredBy() const { return 0; }
CThrownBullet *CBullet::AsThrown() { return 0; }
CProjectileBullet *CBullet::AsProjectile() { return 0; }
void CBullet::GetVelocity(CVector3 &) {}
void CBullet::SetExclusionPair(ISceneNode *) {}
void CBullet::Penetrate(const CCollision &) {}
void CBullet::SetDamage(float) {}
void CBullet::SetBlastRadius(float) {}
void CBullet::SetExplosionParticleSystem(unsigned long, EXPLOSION_PARTICLE_TYPE) {}
