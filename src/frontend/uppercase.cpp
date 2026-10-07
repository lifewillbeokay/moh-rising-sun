// AI-assisted reconstruction from GR8E69; see docs/FEHash.md.
#include "FEHash.h"

char FEUpperCase(char c) {
    if (c >= 'a' && c <= 'z') {
        c -= 32;
    }
    return c;
}
