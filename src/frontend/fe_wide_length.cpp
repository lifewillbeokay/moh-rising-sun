// AI-assisted GR8E69 reconstruction; see docs/research/feng-packages.md.
#ifdef DECOMP_COMPARE
#include "FEngStrings.h"
unsigned long GetStringLength(const short *text) {
    if (!text) return 0;
    unsigned long length = 0;
    while (*text++) ++length;
    return length;
}
#endif
