// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "StringCRC.h"

void GetBaseName(const char *, char *);

// CRC of a path's base name, as used to name assets from script strings.
unsigned int AssetCRC(char *name) {
    char base[256];
    GetBaseName(name, base);
    return GetStringCRC(base);
}
