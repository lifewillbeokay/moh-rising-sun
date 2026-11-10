#ifndef RCMP_MAD_CODEC_H
#define RCMP_MAD_CODEC_H

#include "Rcmp.h"

// The codec's allocation. Both MAD files' .rodata start with its empty label, which is why they share this header.
inline void *RCMP::CODEC::operator new(unsigned int size) {
    return RCMP::rcmp_sys.allocate("", size, 0, 0, (int)RCMP::rcmp_sys.field8);
}

#endif
