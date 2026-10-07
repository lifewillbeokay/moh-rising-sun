// AI-assisted reconstruction from GR8E69; see docs/Bullets.md.
#include "Bullet.h"
void CBullet::Shutdown() {
    field_b0 = 0.0f;
    collisionId = static_cast<EClsnId>(-1);
    field_b4 = 3.402823466e+38f;
}
void BulletBaseDraw();
void CBullet::Draw(CDrawContext &) { BulletBaseDraw(); }
void BulletBaseDraw() {}
unsigned int CBullet::IsDrawEnabled() const { return 1; }
