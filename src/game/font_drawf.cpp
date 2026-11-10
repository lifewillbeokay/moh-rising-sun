// A fragment of the original font.cpp: formatted drawing and the justification adjustment. Names come from the symbol
// table.
#include <stdarg.h>
#include "Font.h"
extern "C" int vsprintf(char *, const char *, va_list);

void CFont::DrawTextF(float x, float y, const char *format, ...) {
    char text[1024];
    va_list args;
    *args = *(__va_list_tag *)__builtin_saveregs(); // Inferred: va_start written out, so the copy is inline.
    vsprintf(text, format, args);
    if (justification)
        ComputeJustification(x, y, text);
    DrawText(x, y, text);
}
