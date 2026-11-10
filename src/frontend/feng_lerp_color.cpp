// AI-assisted reconstruction from GR8E69; see docs/research/feng-animation-lists.md.
#include "FEngInterpolation.h"

void FELerpColor(FEColor &a, FEColor &b, float t, FEColor *base, FEColor *out) {
    out->b = base->b + a.b + (int)((b.b - a.b) * t + 0.5f);
    out->g = base->g + a.g + (int)((b.g - a.g) * t + 0.5f);
    out->r = base->r + a.r + (int)((b.r - a.r) * t + 0.5f);
    out->a = base->a + a.a + (int)((b.a - a.a) * t + 0.5f);
}
