#ifndef GAME_STRING_TABLE_H
#define GAME_STRING_TABLE_H

// AI-assisted GR8E69 reconstruction; see docs/FlexProp.md.
// Names of data fields and this serialized-prefix view are descriptive.
class StringTable {
public:
    virtual char *LookupString(int) = 0;
    static StringTable *Create(char *);
};

struct StringTableData {
    int count;
    char *strings[0]; // Variable tail; stored offsets become pointers in place.
};

class StringTableImpl : public StringTable {
public:
    StringTableData *data;
    StringTableImpl(char *);
    virtual char *LookupString(int); // Original body and vtable remain external.
};

extern StringTable *g_pStringTable;
typedef char StringTableImplSizeCheck[sizeof(StringTableImpl) == 8 ? 1 : -1];
#endif
