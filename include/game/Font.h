#ifndef FONT_H
#define FONT_H
// A scoped view of CFont and of the FONT record it holds. Method names come from the
// original symbols; member names follow what the code does with them.
#include "Color.h"

struct FONT {
    unsigned char unknown00[32];
    unsigned int color;
    unsigned char unknown24[4];
    float depth;
    unsigned char unknown2c[12];
    float scaleX;
    float scaleY;
    unsigned char unknown40[8];
    int field48;
    int field4c;
};

class CScreen;

// The FONT library's text entry points; floats pass in registers.
extern "C" int FONT_getrecta(FONT *font, const unsigned char *text, float *x, float *y, float *w, float *h);
extern "C" int FONT_drawtexta(FONT *font, float x, float y, const unsigned char *text);

class CFont {
public:
    CFont(char *name);
    ~CFont();
    static FONT *CreateHandle(char *name);
    static void DestroyHandle(FONT *font);
    static void SetScreen(CScreen *screen);
    void SetColor(CColor color);
    CColor GetColor();
    void SetDepth(unsigned int depth);
    void GetScale(float &x, float &y);
    void SetScale(float x, float y);
    void SetJustification(unsigned int value);
    unsigned int GetJustification();
    void DrawText(float x, float y, const char *text);
    void DrawTextF(float x, float y, const char *format, ...);
    void GetRenderedSize(const char *text, float &width, float &height);
    void ComputeJustification(float &x, float &y, const char *text);

    FONT *handle;
    int field04;
    int snapY;
    int snapX;
    unsigned int justification;
};
#endif
