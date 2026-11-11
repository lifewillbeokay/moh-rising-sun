// AI-assisted GR8E69 reconstruction; see docs/research/feng-packages.md.
#include "FEngLists.h"
int FEUpperCase(char);
int FEStricmp(const char *a, const char *b) {
    int left, right;
    do { left = FEUpperCase(*a++); right = FEUpperCase(*b++); }
    while (left && left == right);
    return left - right;
}
