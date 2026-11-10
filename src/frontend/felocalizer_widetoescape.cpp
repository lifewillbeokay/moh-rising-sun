// FELocalizer: escaping characters outside the printable range as \\NNN\\.
#include "FELocalizer.h"
// A private helper of FELocalizer.cpp, defined earlier in the original file.
char *ItoA(char *, unsigned long);

void FELocalizer::WideToEscapeString(char *out, const short *in) {
    if (!in) {
        *out = 0;
        return;
    }
    while (*in) {
        if ((unsigned short)*in >= 31 && (unsigned short)*in <= 127) {
            *out++ = *in++;
        } else {
            *out++ = '\\';
            out = ItoA(out, *in++);
            *out++ = '\\';
        }
    }
    *out = 0;
}
