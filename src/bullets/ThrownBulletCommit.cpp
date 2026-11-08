// AI-assisted partial reconstruction from GR8E69.
// See docs/research/explosion-grenade-updates.md.
#include "Bullet.h"
#include "Light.h"
#include "StringCRC.h"
#include "SoundEvents.h"
class BSObject;
class TriggerObject;
int BSSendMessage(unsigned short, BSObject *, TriggerObject *, BSObject *, void *);
void DebugMsg(const char *, ...);
extern "C" float sqrtf(float);

#if defined(DECOMP_COMPARE)
// Comparison-only inline lifetime/initialization bodies. The zeroed fields and
// table stores follow CommitUpdate; their original constructor spelling is unknown.
inline PositionSoundEmitter::PositionSoundEmitter() { x = y = z = 0.0f; }
inline PositionSoundEmitter::~PositionSoundEmitter() {}
inline SoundEvent::SoundEvent() : emitter(0), listener(0), field_08(0), field_1c(0) {
    int *p = field_10;
    for (int i = 3; i != 0; --i)
        *p++ = 0;
    field_0c = 0.0f;
}
inline void SoundEvent::SetEmitter(SoundEmitter *p) { emitter = p; }
inline SoundAliasEvent::SoundAliasEvent() : SoundEvent(), alias(0) {}
inline SoundAliasEvent::~SoundAliasEvent() {}

static inline void SetScaled(CVector3 &r, const CVector3 &v, float s) {
    r.x = v.x * s;
    r.y = v.y * s;
    r.z = v.z * s;
}

void CThrownBullet::CommitUpdate() {
    if (noCatch)
        return;
    switch (type) {
    case 1: case 2: case 4: case 5: case 7: case 9: case 10: case 11:
        if (!(field_b0 <= 0.0f) && (!explodeNow || caught)) {
            position = localToWorld.GetPos();
            spin = velocity;
            if (type != 4)
                bit29 = !bit31;
            bit31 = 0;
            bit30 = 0;
        } else {
            if (!caught) {
                CVector3 pos = localToWorld.GetPos();
                if (type != 5) {
                    CExplosion explosion(g_scene, pos, properties->blastRadius, 1.0f, GetDamage(),
                        (EScriptBulletType)GetScriptBulletType(), GetFiredBy(), false, true);
                }
                CMatrix matrix;
                if (!CMatrix::s_ClassInit)
                    CMatrix::InitClass();
                matrix.BuildTrans(pos);
                CreateExplosionEffect(matrix, bit31);
                PositionSoundEmitter emitter;
                emitter.SetPosition(pos);
                SoundAliasEvent event;
                event.SetEmitter(&emitter);
                event.alias = GetStringCRC("Explo_Grenade");
                event.SendEvent();
                BSSendMessage(141, (BSObject *)GetScriptObject(), 0, 0, this);
            }
            MarkForDestruction(0);
        }
        break;
    case 0: case 6: case 8:
        if (!(field_b0 <= 0.0f) && (!explodeNow || caught)) {
            position = localToWorld.GetPos();
            spin = velocity;
        } else {
            CVector3 pos = localToWorld.GetPos();
            float speed = sqrtf(velocity.LengthSquared());
            if (!(speed <= 0.001f)) {
                CVector3 offset;
                SetScaled(offset, velocity, -0.05f / speed);
                pos += offset;
            }
            CExplosion explosion(g_scene, pos, properties->blastRadius, 1.0f, GetDamage(),
                (EScriptBulletType)GetScriptBulletType(), GetFiredBy(), false, true);
            CMatrix matrix;
            if (!CMatrix::s_ClassInit)
                CMatrix::InitClass();
            matrix.BuildTrans(pos);
            CreateExplosionEffect(matrix, field_b0 > 0.0f);
            PositionSoundEmitter emitter;
            emitter.SetPosition(pos);
            SoundAliasEvent event;
            event.SetEmitter(&emitter);
            event.alias = GetStringCRC("Explo_Bazooka");
            event.SendEvent();
            BSSendMessage(141, (BSObject *)GetScriptObject(), 0, 0, this);
            MarkForDestruction(0);
        }
        break;
    default:
        DebugMsg("WARNING: Unknown thrown bullet type in CommitUpdate.  Type %d\n", type);
        break;
    }
    if (light) {
        CMatrix matrix;
        if (!CMatrix::s_ClassInit)
            CMatrix::InitClass();
        GetTMLocalToWorld(matrix);
        light->SetTMLocalToWorld(matrix);
    }
}
#endif
