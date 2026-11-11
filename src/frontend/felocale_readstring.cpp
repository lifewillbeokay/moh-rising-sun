// AI-assisted GR8E69 reconstruction; see docs/research/feng-packages.md.
#ifdef DECOMP_COMPARE
#include "FELocalizer.h"
int FELocalizer::ReadString(short *out, char *data, unsigned long &offset, unsigned long size) {
    bool escaped = false;
    short value = 0;
    unsigned long i;
    for (i = offset; i < size; ++i) {
        unsigned char c = data[i];
        if (c == '\r' || c == '\n') break;
        if (escaped) {
            if (c == '\\') { *out++ = value; escaped = false; }
            else if ((unsigned long)(c - '0') < 10) value = value * 10 + (char)c - '0';
        } else if (c == '\\' && (i + 1 >= size || data[i + 1] != '\\')) {
            escaped = true; value = 0;
        } else *out++ = (signed char)c;
    }
    *out = 0;
    while (i < size && (data[i] == '\r' || data[i] == '\n')) ++i;
    offset = i;
    return i < size;
}
#endif
