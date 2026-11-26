// AI-assisted GR8E69 reconstruction; see docs/research/save-popups.md.
#include "SaveGamePopups.h"
#include "FengInterface.h"
#include "Screen.h"
#include <math.h>
// DrawPoly's original type tag and observed 24-byte vertex layout. Fields are descriptive.
struct _spritepolyvert {
    float s, t, x, y, z;
    unsigned char r, g, b, a;
};
#ifdef DECOMP_COMPARE
void CSaveGamePopup::Draw() {
    if (!displayed) return;
    engine->Render();
    if (!sprite) return;
    float frame = (float)unknown_14++;
    if (unknown_14 > 32) unknown_14 = 0;
    // Preserve the double scale and the original rounding before the trig calls.
    float angle = frame * 0.19039955476301776;
    float widthScale = CScreen::m_sWidthScale;
    float halfWidth = widthScale * (cosf(angle) * 16.0f);
    sinf(angle);
    float halfHeight = CScreen::m_sHeightScale * 16.0f;
    float center = CScreen::m_sVideoMode == 0 ? 95.0f : 75.0f;
    _spritepolyvert left[4], right[4];
    memset(&left[0], 0, sizeof(_spritepolyvert));
    left[0].x = center * CScreen::m_sWidthScale - halfWidth;
    left[0].y = CScreen::m_sHeightScale * 110.0f - halfHeight;
    left[0].z = 1.0f;
    left[0].r = left[0].g = left[0].b = 128; left[0].a = 255;
    memset(&left[1], 0, sizeof(_spritepolyvert));
    left[1].x = center * CScreen::m_sWidthScale + halfWidth;
    left[1].y = CScreen::m_sHeightScale * 110.0f - halfHeight;
    left[1].s = 1.0f;
    left[1].z = 1.0f;
    left[1].r = left[1].g = left[1].b = 128; left[1].a = 255;
    memset(&left[2], 0, sizeof(_spritepolyvert));
    left[2].x = center * CScreen::m_sWidthScale + halfWidth;
    left[2].y = CScreen::m_sHeightScale * 110.0f + halfHeight;
    left[2].s = 1.0f;
    left[2].t = 1.0f;
    left[2].z = 1.0f;
    left[2].r = left[2].g = left[2].b = 128; left[2].a = 255;
    memset(&left[3], 0, sizeof(_spritepolyvert));
    left[3].x = center * CScreen::m_sWidthScale - halfWidth;
    left[3].y = CScreen::m_sHeightScale * 110.0f + halfHeight;
    left[3].t = 1.0f;
    left[3].z = 1.0f;
    left[3].r = left[3].g = left[3].b = 128; left[3].a = 255;
    center = (640.0f - center) - 5.0f;
    memset(&right[0], 0, sizeof(_spritepolyvert));
    right[0].x = center * CScreen::m_sWidthScale - halfWidth;
    right[0].y = CScreen::m_sHeightScale * 110.0f - halfHeight;
    right[0].z = 1.0f;
    right[0].r = right[0].g = right[0].b = 128; right[0].a = 255;
    memset(&right[1], 0, sizeof(_spritepolyvert));
    right[1].x = center * CScreen::m_sWidthScale + halfWidth;
    right[1].y = CScreen::m_sHeightScale * 110.0f - halfHeight;
    right[1].s = 1.0f;
    right[1].z = 1.0f;
    right[1].r = right[1].g = right[1].b = 128; right[1].a = 255;
    memset(&right[2], 0, sizeof(_spritepolyvert));
    right[2].x = center * CScreen::m_sWidthScale + halfWidth;
    right[2].y = CScreen::m_sHeightScale * 110.0f + halfHeight;
    right[2].s = 1.0f;
    right[2].t = 1.0f;
    right[2].z = 1.0f;
    right[2].r = right[2].g = right[2].b = 128; right[2].a = 255;
    memset(&right[3], 0, sizeof(_spritepolyvert));
    right[3].x = center * CScreen::m_sWidthScale - halfWidth;
    right[3].y = CScreen::m_sHeightScale * 110.0f + halfHeight;
    right[3].t = 1.0f;
    right[3].z = 1.0f;
    right[3].r = right[3].g = right[3].b = 128; right[3].a = 255;
    sprite->DrawPoly(left, 4, (CSprite::EPolyPrimType)5);
    sprite->DrawPoly(right, 4, (CSprite::EPolyPrimType)5);
}
#endif
