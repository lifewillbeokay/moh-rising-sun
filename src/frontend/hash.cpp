// AI-assisted reconstruction from GR8E69; see docs/FEHash.md.
#include "FEHash.h"

unsigned int FEHashUpper(const char *text) {
    unsigned int hash = 0xffffffff;
    if (text) {
        while (*text) {
            hash += hash << 5;
            hash += static_cast<unsigned char>(FEUpperCase(*text));
            ++text;
        }
    }
    return hash;
}
