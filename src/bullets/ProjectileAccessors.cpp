// AI-assisted reconstruction from GR8E69; see docs/Bullets.md.
#include "Bullet.h"
float CProjectileBullet::GetDamage() const { return properties->damage; }
ISceneNode *CProjectileBullet::GetFiredBy() const { return firedBy; }
CProjectileBullet *CProjectileBullet::AsProjectile() { return this; }
void CProjectileBullet::SetDrawForward(bool forward) { flags.drawForward = forward; }
