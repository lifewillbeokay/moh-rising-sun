// A fragment of the original font.cpp: moving a text position by its justification flags. Names come from the symbol
// table; what the flag bits mean is inferred from the code.
#include "Font.h"
void CFont::ComputeJustification(float &x, float &y, const char *text) {
    float width, height;
    GetRenderedSize(text, width, height);
    if (justification & 2)
        x -= width * 0.5f;
    else if (justification & 1)
        x -= width;
    if (justification & 8)
        y -= height * 0.5f;
    else if (justification & 4)
        y -= height;
}
