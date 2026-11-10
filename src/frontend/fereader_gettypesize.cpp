// FEPackageReader.cpp: reading a package file's chunks; values in the file are little-endian.
#include "FEngLists.h"
static inline unsigned long FESwap(unsigned long v) {
    return (v >> 24) | (v << 24) | ((v << 8) & 0xff0000) | ((v >> 8) & 0xff00);
}
struct FEChunk {
    unsigned long id;   // +0x0
    unsigned long size; // +0x4
};
struct FETypeSize {
    unsigned long type; // +0x0
    unsigned long size; // +0x4
};
class FEPackageReader {
  public:
    FEPackageReader();
    ~FEPackageReader();
    void Reset();
    FEChunk *FindChild(FEChunk *, unsigned long);
    unsigned long GetTypeSize(unsigned long);
    int ReadTypeSizes();
    unsigned long unknown_0;   // +0x00
    FEChunk *root;             // +0x04
    unsigned char unknown_8[0x14 - 8];
    unsigned long unknown_14;  // +0x14
    unsigned long unknown_18;  // +0x18
    unsigned long unknown_1c;  // +0x1c
    unsigned char unknown_20[0x38 - 0x20];
    unsigned long typeCount;   // +0x38
    FETypeSize *types;         // +0x3c
    unsigned char unknown_40[0x48 - 0x40];
    unsigned long unknown_48;  // +0x48
    unsigned long unknown_4c;  // +0x4c
    unsigned long unknown_50;  // +0x50
    unsigned long unknown_54;  // +0x54
};

unsigned long FEPackageReader::GetTypeSize(unsigned long type) {
    for (unsigned long i = 0; i < typeCount; i++) {
        if (FESwap(types[i].type) == type) return FESwap(types[i].size);
    }
    return 0;
}
