// CConfigFile: a text file split into "[section]" blocks, with "key = value" lookups in the
// current section. The member names follow what the code does with them.
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "ConfigFile.h"

extern "C" int strcasecmp(const char *, const char *);
extern "C" int strncasecmp(const char *, const char *, size_t);

void *TLT_LoadFileNormal(const char *, int *, int);
void TLT_CloseFile(void *);
void DebugMsg(const char *, ...);

static char *SkipToEOL(char *p) {
    if (p) {
        char c;
        do {
            c = *p++;
            if (c == 0) {
                p--;
                return p;
            }
        } while (c != '\n' && c != '\r');
        if (c == '\r' && *p == '\n')
            p++;
    }
    return p;
}

static void CopyStringData(char *dest, char *source, int size) {
    if (dest && size) {
        size--;
        char *p = source;
        char c;
        do {
            c = *p++;
        } while (c != 0 && c != '\n' && c != '\r');
        p--;
        int length = p - source;
        if (length > size)
            length = size;
        memcpy(dest, source, length);
        dest[length] = 0;
    }
}

CConfigFile::CConfigFile(const char *fileName) {
    int size;
    count = 0;
    current = 0;
    data = 0;
    char *file = (char *)TLT_LoadFileNormal(fileName, &size, 0);
    if (file && size) {
        data = new char[size + 1];
        if (data) {
            memcpy(data, file, size);
            data[size] = 0;
            char *p = strchr(data, '[');
            CConfigSection *section = sections;
            while (p) {
                p++;
                char *end = strchr(p, ']');
                if (!end) {
                    DebugMsg("CConfigFile::CConfigFile() -- Section name %s doesn't terminate ( ] )", p);
                    break;
                }
                if (count >= 64) {
                    DebugMsg("CConfigFile(%s) has more than %d sections\n", fileName, 64);
                    break;
                }
                section->name = p;
                *end = 0;
                end++;
                section->start = end;
                p = strchr(end, '[');
                if (p) {
                    *p = 0;
                    section->end = p;
                } else {
                    section->end = data + size;
                }
                count++;
                section++;
            }
        }
    }
    if (file)
        TLT_CloseFile(file);
}

CConfigFile::~CConfigFile() {
    if (data)
        delete data;
}

int CConfigFile::SetSection(const char *name) {
    int i = count;
    if (i) {
        CConfigSection *section = sections;
        do {
            if (strcasecmp(section->name, name) == 0) {
                current = section;
                return 1;
            }
            section++;
        } while (--i);
    }
    current = 0;
    return 0;
}

char *CConfigFile::GetVar(const char *key) {
    if (current) {
        char *found = strstr(current->start, key);
        if (found) {
            char *eol = SkipToEOL(found);
            if (!eol)
                eol = current->end;
            char *equals = strchr(found, '=');
            if (equals && equals < eol) {
                for (equals++; isspace(*equals); equals++)
                    ;
                return equals;
            }
        }
    }
    return 0;
}

int CConfigFile::AsInt(const char *key, int &value) {
    int found = 0;
    char *text = GetVar(key);
    if (text) {
        value = atoi(text);
        found = 1;
    }
    return found;
}

int CConfigFile::AsBool(const char *key, bool &value) {
    int found = 0;
    char *text = GetVar(key);
    if (text) {
        if (strncasecmp(text, "true", 4) == 0)
            value = 1;
        else
            value = found;
        found = 1;
    }
    return found;
}

int CConfigFile::AsString(const char *key, char *buffer, int size) {
    char *text = GetVar(key);
    if (text) {
        CopyStringData(buffer, text, size);
        return 1;
    }
    return 0;
}
