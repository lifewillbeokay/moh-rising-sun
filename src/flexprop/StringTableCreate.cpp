// AI-assisted reconstruction from GR8E69; see docs/FlexProp.md.
#include "StringTable.h"
extern "C" void *DWI_alloc(const char *, int, int);
inline void *operator new(unsigned int, void *p) { return p; }

StringTable *StringTable::Create(char *base) {
    return new (DWI_alloc("source/ai_script/flexprop.cpp:700",
                         sizeof(StringTableImpl), 1024)) StringTableImpl(base);
}
