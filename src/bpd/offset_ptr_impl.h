#ifndef BPD_OFFSET_PTR_IMPL_H
#define BPD_OFFSET_PTR_IMPL_H
#include "OffsetPtr.h"

// Target-specific 32-bit pointer representation, preserving null offsets.
template <class T> void offsetPtr(T *&pointer, int base) {
    if (pointer)
        pointer = (T *)((int)pointer + base);
}
#endif
