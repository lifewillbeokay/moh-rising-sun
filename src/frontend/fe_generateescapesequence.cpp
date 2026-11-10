// FEWideString: FEng's wide-character string; characters are 16 bits, with the top bit
// marking an escape code. See docs/Frontend.md.
#include "FEngStrings.h"
#include <string.h>
struct FEEscapeSequence { const char *name; short code; };
extern FEEscapeSequence SpecialColorCharacters[];

static void GenerateEscapeSequence(char *out, const char *name, unsigned long value) {
    *out++ = '\\';
    if (name) {
        while (*name) *out++ = *name++;
    }
    unsigned long divisor = 1;
    while (value >= divisor * 10) divisor *= 10;
    do {
        unsigned long digit = value / divisor;
        *out++ = digit + '0';
        value -= digit * divisor;
        divisor /= 10;
    } while (divisor);
    *out = '\\';
    out[1] = divisor;
}
