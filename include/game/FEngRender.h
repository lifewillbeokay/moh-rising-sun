// What FEng's two game interfaces keep per object while drawing. Scoped view; see docs/Frontend.md.
#ifndef GAME_FENG_RENDER_H
#define GAME_FENG_RENDER_H
#include "FEColor.h"
#include "EAGLTransform.h"
// Object kinds by what the render switch does with each. The names are inferred; the range is what
// the switch's comparison tree shows: it leaves out the last value's test, so the type ends at 12.
enum FEObjectKind {
    FEObjectKindInvalid = -1,
    FEObjectKindNone,
    FEObjectKindImage,
    FEObjectKindString,
    FEObjectKindModel,
    FEObjectKindList,
    FEObjectKindGroup,
    FEObjectKindCodeList,
    FEObjectKindMovie,
    FEObjectKindEffect,
    FEObjectKindColoredImage,
    FEObjectKindUnknown10,
    FEObjectKindSimpleImage,
    FEObjectKindCount
};
// A color as four floats from 0 to 1. Name inferred.
struct SFengColor {
    float a, r, g, b;
    SFengColor() { b = g = r = a = 0.0f; }
    void operator=(const FEColor &color) {
        int red = color.r;
        int green = color.g;
        int blue = color.b;
        int alpha = color.a;
        r = red * (1.0f / 255.0f);
        g = green * (1.0f / 255.0f);
        b = blue * (1.0f / 255.0f);
        a = alpha * (1.0f / 255.0f);
    }
    void operator*=(SFengColor other) {
        a *= other.a;
        r *= other.r;
        g *= other.g;
        b *= other.b;
    }
};
// One object's drawing state, indexed by its render context number. Name inferred.
struct SFengRenderContext {
    EAGL::Transform transform;  // +0x00
    SFengColor color;           // +0x40
    unsigned char unknown_50[16];
};
#endif
