// AI-assisted partial reconstruction from GR8E69.
// See docs/research/explosion-grenade-updates.md.
#include "Bullet.h"
#include "CameraShake.h"
#include "SceneObjects.h"
extern "C" float sqrtf(float);

#if defined(DECOMP_COMPARE)
// Descriptive helper; its returned copy preserves the observed temporaries.
static inline CVector3 SetCross(CVector3 &out, const CVector3 &a, const CVector3 &b) {
    out.x = a.y * b.z - a.z * b.y;
    out.y = a.z * b.x - a.x * b.z;
    out.z = a.x * b.y - a.y * b.x;
    return out;
}

bool CExplosion::CheckForObstructions(ISceneNode &node) {
    CVector3 center = sphere.GetCenter();
    CVector3 direction, position;
    node.GetPosition(position);
    direction.x = position.x - center.x;
    direction.y = position.y - center.y;
    direction.z = 0.0f;
    float length = sqrtf(direction.LengthSquaredXY());
    if (length != 0.0f)
        direction *= 1.0f / length;
    if (length < 0.001f)
        return false;

    CVector3 right, up;
    SetCross(right, direction, CVector3(0.0f, 0.0f, 1.0f));
    CMatrix matrix;
    if (!CMatrix::s_ClassInit)
        CMatrix::InitClass();
    matrix.SetPos(CVector3(0.0f, 0.0f, 0.0f));
    matrix.SetRight(right);
    matrix.SetFront(direction);
    SetCross(up, right, direction);
    matrix.SetUp(up);

    CVector3 start, end;
    float width = 0.25f, height = 1.6f, scale = 1.0f;
    const float half = 0.5f, low = 0.01f;
    if (node.AsPlayerObject()) {
        if (node.AsPlayerObject()->IsPlayerCrouching())
            scale = 0.33f;
        // Only this float at player +0x18c is established, not the volume type.
        position.z -= *(float *)(node.AsPlayerObject()->localVolume + 12);
    } else if (node.AsSoldierObject()) {
        CMatrix look;
        if (!CMatrix::s_ClassInit)
            CMatrix::InitClass();
        CVector3 head;
        node.AsSoldierObject()->GetLookMatrix(look);
        head = look.GetPos();
        float offset = head.z - position.z;
        if (!(offset >= 0.0f))
            offset = 0.0f;
        scale = offset / height;
    }

    int directionSign = ShouldHeightCheckBeFlipped(node) ? -1 : 1;
    float sign = directionSign;
    height *= scale;
    start.Set(0.0f, 0.0f, height * sign);
    end.Set(width, 0.0f, height);
    if (!TestRay(position, start, end, matrix))
        return false;
    start.Set(0.0f, 0.0f, height * (sign * half));
    end.Set(-width, 0.0f, height * half);
    if (!TestRay(position, start, end, matrix))
        return false;
    start.Set(0.0f, 0.0f, height * (sign * low));
    end.Set(0.0f, 0.0f, height * low);
    if (TestRay(position, start, end, matrix))
        return true;
    return false;
}
#endif
