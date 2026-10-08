// AI-assisted reconstruction from GR8E69; see docs/Paths.md.
#include "CameraShake.h"
void CPlayerObject::MoveOnPath(CAISplinePath *move, CAISplinePath *look, float duration) {
    pathFlags |= 0x80000;
    movePath = move;
    lookPath = look;
    pathParameter = 0.0f;
    pathRate = (1.0f / duration) * 0.016683351f;
}
void CPlayerObject::StopPath() {
    pathFlags &= ~0x80000;
}
