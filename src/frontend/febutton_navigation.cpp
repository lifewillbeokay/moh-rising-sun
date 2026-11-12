// AI-assisted GR8E69 reconstruction; see docs/research/feng-lifecycle.md.
#ifdef DECOMP_COMPARE
#include "FEngInterpolation.h"
#include "ShellManager.h"
#include <math.h>
extern FEVector2 DirectionVectors[], PassOffsets[];
extern unsigned long PassWrapMode[];
FEObject *FEButtonMap::GetButtonFrom(FEObject *current, long direction, FEGameInterface *game, FEButtonWrapMode wrap) {
    FEVector2 origin, shifted, candidate;
    origin.x = origin.y = shifted.x = shifted.y = candidate.x = candidate.y = 0.0f;
    float best = 1e30f;
    unsigned long winner = 0;
    ComputeButtonLocation(current, game, origin);
    for (unsigned long pass = 0; pass < 5; ++pass) {
        if (pass && !(PassWrapMode[pass] & wrap)) continue;
        shifted.x = origin.x + PassOffsets[pass].x; shifted.y = origin.y + PassOffsets[pass].y;
        for (unsigned long i = 0; i < count; ++i) {
            FEObject *object = buttons[i];
            if ((object->flags & 0x4000000) || object == current) continue;
            ComputeButtonLocation(object, game, candidate);
            float dx = candidate.x - shifted.x, dy = candidate.y - shifted.y;
            float distance = sqrtf(dx * dx + dy * dy);
            if (distance < 0.0001f) continue;
            float dot = dx * (1.0f / distance) * DirectionVectors[direction].x +
                        dy * (1.0f / distance) * DirectionVectors[direction].y;
            if (!(dot < 0.0f)) dot *= dot;
            float cost;
            if (!(dot < 0.25f)) cost = (1.0f - dot) * 200.0f + distance;
            else cost = 1500.0f;
            if (!(cost >= best)) { winner = i; best = cost; }
        }
    }
    return !(best >= 1500.0f) ? buttons[winner] : 0;
}
void FEButtonMap::ComputeButtonLocation(FEObject *object, FEGameInterface *game, FEVector2 &out) {
    FEMatrix4 context;
    FEObjData *data = (FEObjData *)object->data;
    if (game && object->renderContext && game->GetContextTransform(object->renderContext, context)) {
        FEVector3 transformed(0, 0, 0);
        FEMultMatrix(&transformed, &context, &data->position);
        out.x = transformed.x; out.y = transformed.y;
    } else { out.x = data->position.x; out.y = data->position.y; }
}
#endif
