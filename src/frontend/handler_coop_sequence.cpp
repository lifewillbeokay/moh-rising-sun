// AI-assisted GR8E69 reconstruction; see docs/research/frontend-screens.md.
#ifdef DECOMP_COMPARE
#include "PackageHandlers.h"
int HandlerCoopGameOptions::GetNextEntryInSequence(int *sequence, unsigned int count, int direction, int current) {
    if (!count || !sequence) return current;
    int last = count - 1;
    int step = -1, index = last, result = sequence[0];
    if (direction < 0) { step = 1; index = 0; result = sequence[last]; }
    for (; index >= 0 && index <= last; index += step) {
        int value = sequence[index];
        if (value == current) return result;
        result = value;
    }
    return result;
}
#endif
