// A fragment of the original drivecheck.cpp: drawing an error message with the ROM font. This is the SDK's
// DEMOPuts code with the z coordinate dropped. Names come from the symbol table; locals are inferred.
#include <dolphin/gx/GXGeometry.h>
#include <dolphin/gx/GXTev.h>
#include <dolphin/gx/GXPixel.h>
#include <dolphin/gx/GXLighting.h>
#include <dolphin/os/OSFont.h>

// File statics in the original, defined outside this fragment; so is LoadSheet.
extern OSFontHeader *g_pFontData;
extern void *g_pLastSheet;
extern short g_fontSize;
extern short g_fontSpace;

volatile short gGXFifoS16 __attribute__((address(0xcc008000)));
static inline void GXPosition3s16(short x, short y, short z) { gGXFifoS16 = x; gGXFifoS16 = y; gGXFifoS16 = z; }
static inline void GXTexCoord2s16(short s, short t) { gGXFifoS16 = s; gGXFifoS16 = t; }

void LoadSheet(void *image, GXTexMapID texMapID);

static void DrawFontChar(int x, int y, int xChar, int yChar) {
    short posLeft = x;
    short posRight = posLeft + g_fontSize;
    short posTop = y - g_pFontData->ascent * g_fontSize / g_pFontData->cellWidth;
    short posBottom = y + g_pFontData->descent * g_fontSize / g_pFontData->cellWidth;
    short texLeft = xChar;
    short texTop = yChar;
    short texRight = xChar + g_pFontData->cellWidth;
    short texBottom = yChar + g_pFontData->cellHeight;
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3s16(posLeft, posTop, 0);
    GXTexCoord2s16(texLeft, texTop);
    GXPosition3s16(posRight, posTop, 0);
    GXTexCoord2s16(texRight, texTop);
    GXPosition3s16(posRight, posBottom, 0);
    GXTexCoord2s16(texRight, texBottom);
    GXPosition3s16(posLeft, posBottom, 0);
    GXTexCoord2s16(texLeft, texBottom);
}

static int RFPuts(short x, short y, const char *string) {
    if (g_pFontData) {
        g_pLastSheet = 0;
        int width = 0;
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 4);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S16, 0);
        x *= 16;
        y *= 16;
        while (*string) {
            if (*string == '\n') {
                width = 0;
                y += g_pFontData->leading * g_fontSize / g_pFontData->cellWidth;
                string++;
            } else {
                void *image;
                s32 xChar, yChar, charWidth;
                string = OSGetFontTexture(string, &image, &xChar, &yChar, &charWidth);
                LoadSheet(image, GX_TEXMAP0);
                DrawFontChar(x + width, y, xChar, yChar);
                width += g_fontSize * charWidth / g_pFontData->cellWidth + g_fontSpace;
            }
        }
        return (width + 15) / 16;
    }
    return 0;
}

static int GetRFTextWidth(const char *string) {
    if (g_pFontData) {
        int widest = 0;
        int width = 0;
        while (*string) {
            if (*string == '\n') {
                if (widest < width)
                    widest = width;
                width = 0;
            }
            s32 charWidth;
            string = OSGetFontWidth(string, &charWidth);
            width += g_fontSize * charWidth / g_pFontData->cellWidth + g_fontSpace;
        }
        if (widest < width)
            widest = width;
        return (widest + 15) / 16;
    }
    return 0;
}

void DrawMessage(int width, int y, const char *message) {
    GXSetNumTevStages(1);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    int x = (width - GetRFTextWidth(message)) / 2;
    int size = g_fontSize / 16;
    y += g_pFontData->leading * size / g_pFontData->cellWidth;
    GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_CLEAR);
    RFPuts(x, y, message);
}
