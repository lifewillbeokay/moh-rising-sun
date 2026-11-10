// AI-assisted reconstruction from GR8E69; see docs/Bullets.md.
#include "Bullet.h"
// The instantiations live in the original's template section; only the property
// table and the 64-byte extent (the factory's member spacing) are observed.
template <class T, class P, class E> class CBulletFactoryHelper {
  public:
    P *properties;
    unsigned char unknown_4[64 - 4];
    CBulletFactoryHelper();
    void Init(P *);
    void Reset();
    void Shutdown();
    T *CreateBullet(E, const CVector3 &, const CVector3 &, float, EClsnId, float, ISceneNode *);
    void DestroyBullet(T *);
};
typedef CBulletFactoryHelper<CProjectileBullet, ProjectileBulletProperties_struct, EProjectileBulletTypes> ProjectileHelper;
typedef CBulletFactoryHelper<CThrownBullet, ThrownBulletProperties_struct, EThrownBulletTypes> ThrownHelper;
class CBulletFactory {
  public:
    ProjectileBulletProperties_struct *projectileProperties;
    ThrownBulletProperties_struct *thrownProperties;
    unsigned char unknown_8[16 - 8];
    ProjectileHelper projectiles;
    ProjectileHelper mountedProjectiles; // Used for collision id 3 (name inferred).
    ThrownHelper thrown;
    int initialized;
    CBulletFactory();
    void Init();
    void Reset();
    void Shutdown();
    CBullet *CreateBullet(int, bool, const CVector3 &, const CVector3 &, float, EClsnId, float, ISceneNode *);
    void DestroyBullet(CBullet *);
    int GetBulletByCRC(unsigned int);
    void *GetBulletSprite(bool, int);
    void LoadBulletParameters();
    void LoadBulletGUISprites();
    void DeleteBulletGUISprites();
};
CBulletFactory::CBulletFactory() {
    initialized = 0;
    projectileProperties = 0;
    thrownProperties = 0;
}
void CBulletFactory::Init() {
    LoadBulletParameters();
    LoadBulletGUISprites();
    projectiles.Init(projectileProperties);
    mountedProjectiles.Init(projectileProperties);
    thrown.Init(thrownProperties);
    initialized = 1;
}
void CBulletFactory::Reset() {
    projectiles.Reset();
    mountedProjectiles.Reset();
    thrown.Reset();
}
void CBulletFactory::Shutdown() {
    DeleteBulletGUISprites();
    projectiles.Shutdown();
    mountedProjectiles.Shutdown();
    thrown.Shutdown();
    delete[] thrownProperties;
    delete[] projectileProperties;
    thrownProperties = 0;
    projectileProperties = 0;
    initialized = 0;
}
CBullet *CBulletFactory::CreateBullet(int type, bool projectile, const CVector3 &position, const CVector3 &direction, float speed, EClsnId id, float scale, ISceneNode *owner) {
    if (projectile) {
        if (id == 3) return mountedProjectiles.CreateBullet((EProjectileBulletTypes)type, position, direction, speed, (EClsnId)3, scale, owner);
        return projectiles.CreateBullet((EProjectileBulletTypes)type, position, direction, speed, id, scale, owner);
    }
    return thrown.CreateBullet((EThrownBulletTypes)type, position, direction, speed, id, scale, owner);
}
void CBulletFactory::DestroyBullet(CBullet *bullet) {
    if (bullet->bulletKind == 1) {
        if (bullet->GetCollisionId() == 3) mountedProjectiles.DestroyBullet((CProjectileBullet *)bullet);
        else projectiles.DestroyBullet((CProjectileBullet *)bullet);
    } else {
        thrown.DestroyBullet((CThrownBullet *)bullet);
    }
}
int CBulletFactory::GetBulletByCRC(unsigned int crc) {
    int i;
    ProjectileBulletProperties_struct *p = projectiles.properties;
    for (i = 0; i < 36; p++, i++)
        if (p->crc == crc) return i;
    ThrownBulletProperties_struct *t = thrown.properties;
    for (i = 0; i < 12; t++, i++)
        if (t->crc == crc) return i;
    return -1;
}
void *CBulletFactory::GetBulletSprite(bool projectile, int index) {
    if (projectile) return projectileProperties[index].sprite;
    return thrownProperties[index].sprite;
}
