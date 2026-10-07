// AI-assisted reconstruction from GR8E69; see docs/Bullets.md.
#include "Bullet.h"
float CThrownBullet::GetDamage() const { return properties->damage; }
ISceneNode *CThrownBullet::GetFiredBy() const { return firedBy; }
CThrownBullet *CThrownBullet::AsThrown() { return this; }
