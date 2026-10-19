// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "CRC.h"

extern "C" char *strncpy(char *, const char *, unsigned int);
extern "C" char *strlwr(char *);
extern "C" void *DWI_alloc(const char *, int, int);
inline void *operator new(unsigned int, void *p) { return p; }

void BIFunc_GetAnimatedTextureCRC(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    char buffer[1024];
    strncpy(buffer, (const char *)args[1], 1024);
    strlwr(buffer);
    CCRC *crc = new (DWI_alloc("source/system/crc.h:241", sizeof(CCRC), 1024)) CCRC;
    crc->width = 32;
    crc->polynomial = 0x04c11db7;
    crc->reflect = 1;
    crc->initial = 0xffffffff;
    crc->finalXor = 0xffffffff;
    crc->mask = 0xffffffff;
    crc->Reset();
    char *p = buffer;
    while (*p)
        *crc += (unsigned char)*p++;
    unsigned int value;
    if (crc->reflect)
        value = crc->finalXor ^ crc->u4Reflect(crc->value, crc->width);
    else
        value = crc->finalXor ^ crc->value;
    delete crc;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = value;
}
