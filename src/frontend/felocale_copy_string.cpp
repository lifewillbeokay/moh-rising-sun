// AI-assisted GR8E69 reconstruction; private FELocalizer.cpp helper.
// See docs/research/feng-packages.md.
#include "FEngStrings.h"
#include <string.h>
static void FECopyString(char *&out, const char *text) {
    if (out) delete[] out;
    out = 0;
    if (text) {
        out = new ("source/FEngine/FELocalizer.cpp:25") char[strlen(text) + 1];
        strcpy(out, text);
    }
}
