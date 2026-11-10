// FELocalizer: reading the next line that holds a printable character.
#include "FELocalizer.h"

int FELocalizer::ReadNextString(short *out, char *data, unsigned long &offset, unsigned long size) {
    int more;
    do {
        more = ReadString(out, data, offset, size);
        int i = 0;
        while (out[i] <= 31 && out[i]) i++;
        if (out[i]) break;
    } while (more == 1);
    return more;
}
