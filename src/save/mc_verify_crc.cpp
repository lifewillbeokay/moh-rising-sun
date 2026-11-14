// AI-assisted GR8E69 reconstruction; see docs/research/save-flow.md.
#include "MemoryCardFlow.h"
#ifdef DECOMP_COMPARE
bool CMemoryCard::ScratchpadDataOkay() const {
    CCRC *crc = new ("source/system/crc.h:241") CCRC;
    crc->width = 32; crc->polynomial = 0x04c11db7; crc->reflect = 1;
    crc->initial = 0xffffffff; crc->finalXor = 0xffffffff; crc->mask = 0xffffffff;
    crc->Reset();
    unsigned int count = g_uGameDataSize;
    unsigned char *bytes = g_pScratchpad;
    for (; count; --count) *crc += *bytes++;
    unsigned int *trailer = (unsigned int *)(g_pScratchpad + g_uGameDataSize);
    unsigned int value;
    if (crc->reflect) value = crc->finalXor ^ crc->u4Reflect(crc->value, crc->width);
    else value = crc->finalXor ^ crc->value;
    delete crc;
    if (trailer[0] == value) return trailer[1] == (unsigned int)SPlayerProfile::VERSION;
    else return false;
}
#endif
