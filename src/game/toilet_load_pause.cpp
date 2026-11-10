// A fragment of the original toilet.cpp. Names come from the symbol table. Each state returns how far
// to advance.
#include "Toilet.h"

bool TLT_LoadFile_Async_Load(char **, int *);

static int TLT_Load_WaitPause() {
    char *buffer;
    int size;
    if (!TLT_LoadFile_Async_Load(&buffer, &size)) {
        g_toilet.pauseBigFile = buffer;
        g_toilet.pauseBigFileSize = size;
        return 1;
    }
    return 0;
}
