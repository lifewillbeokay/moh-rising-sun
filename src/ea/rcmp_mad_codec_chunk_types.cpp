// A MAD movie chunk tag test (rcmp_mad_codec_chunk_types.cpp). The function's name comes from the symbol table. The table
// is file-local and named ChunkTypes in the original, in an unnamed namespace there.
#include "RcmpMadCodec.h"

namespace RCMP {
int MAD_CODEC_is_chunk_for_codec(unsigned int);
}

static const unsigned int ChunkTypes[3] = {0x4d41446d, 0x4d414465, 0x4d41446b};

int RCMP::MAD_CODEC_is_chunk_for_codec(unsigned int type) {
    for (unsigned int i = 0; i < 3; i++) {
        if (ChunkTypes[i] == type)
            return 1;
    }
    return 0;
}
