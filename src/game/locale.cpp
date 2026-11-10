// A fragment of the original locale.cpp: EA's little-endian locale string files. LOCALE_ names and
// LOCALE_INDEX come from the symbol table; field names and the macro's spelling are inferred.
#include <stdlib.h>

static inline int READ16(const void *address) {
    const unsigned char *p = (const unsigned char *)address;
    return (p[1] << 8) | p[0];
}
static inline unsigned int READ32(const void *address) {
    const unsigned char *p = (const unsigned char *)address;
    return (p[3] << 24) | (p[2] << 16) | (p[1] << 8) | p[0];
}

struct LOCALE_INDEX {
    unsigned char unknown00[8];
    unsigned int count; // +0x08
    unsigned char unknown0c[4];
    unsigned short entries[1][2]; // +0x10; id, then string index. Extent is count.
};
struct LOCALE_TABLE {
    unsigned char unknown00[12];
    unsigned int count; // +0x0c
    unsigned int offsets[1]; // +0x10; extent is count.
};
struct LOCALE {
    unsigned char unknown00[4];
    unsigned int indexOffset; // +0x04
    unsigned char flags;      // +0x08; bit 0: strings are looked up through the index.
    unsigned char unknown09[3];
    unsigned short tableCount;   // +0x0c
    unsigned short currentTable; // +0x0e
    unsigned int tables[1];      // +0x10; extent is tableCount.
};

extern "C" int LOCALE_getstate(LOCALE *locale, int state) {
    int result;
    switch (state) {
    case 0:
        result = READ16(&locale->currentTable);
        break;
    case 1:
        result = READ16(&locale->tableCount);
        break;
    case 2: {
        LOCALE_TABLE *table = (LOCALE_TABLE *)((char *)locale + READ32(&locale->tables[READ16(&locale->currentTable)]));
        result = READ32(&table->count);
        break;
    }
    case 3:
        result = locale->flags & 1;
        break;
    default:
        result = 0;
        break;
    }
    return result;
}

extern "C" void LOCALE_setstate(LOCALE *locale, int state, int value) {
    if (state == 0) {
        locale->currentTable = value;
        ((unsigned char *)&locale->currentTable)[0] = value;
        ((unsigned char *)&locale->currentTable)[1] = (unsigned int)value >> 8;
    }
}

static int compare(const void *a, const void *b) {
    return *(const unsigned short *)a - READ16(b);
}

static int getstringidbyindex(unsigned short id, const LOCALE_INDEX *index) {
    unsigned short key = id;
    unsigned short *found = (unsigned short *)bsearch(&key, index->entries, READ32(&index->count), 4, compare);
    int result;
    if (found) {
        found++;
        result = READ16(found);
    } else
        result = -1;
    return result;
}

extern "C" char *LOCALE_getstr(LOCALE *locale, int id) {
    if (locale->flags & 1)
        id = getstringidbyindex(id, (LOCALE_INDEX *)((char *)locale + READ32(&locale->indexOffset)));
    char *text = (char *)locale + READ32(&locale->tables[READ16(&locale->currentTable)]);
    if (id >= 0 && (unsigned int)id < READ32(&((LOCALE_TABLE *)text)->count))
        text += READ32(&((LOCALE_TABLE *)text)->offsets[id]);
    else
        text = 0;
    return text;
}

extern "C" void LOCALE_create(LOCALE *locale, int swap) {
    if (swap) {
        int tableCount = READ16(&locale->tableCount);
        for (int i = 0; i < tableCount; i++) {
            LOCALE_TABLE *table = (LOCALE_TABLE *)((char *)locale + READ32(&locale->tables[i]));
            int count = READ32(&table->count);
            for (int j = 0; j < count; j++) {
                unsigned short *text = (unsigned short *)((char *)table + READ32(&table->offsets[j]));
                while (*text) {
                    *text = READ16(text);
                    text++;
                }
            }
        }
    }
}
