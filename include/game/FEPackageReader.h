// FEPackageReader: builds an FEPackage from a package file. Values in the file are little-endian.
// Class and method names are from the symbol table. Member names are inferred from use.
#ifndef GAME_FEPACKAGEREADER_H
#define GAME_FEPACKAGEREADER_H

#include "FEngLists.h"

class FEngine;

static inline unsigned long FESwap(unsigned long v) {
    return (v >> 24) | (v << 24) | ((v << 8) & 0xff0000) | ((v >> 8) & 0xff00);
}
static inline unsigned short FESwapShort(unsigned short v) { return (v << 8) | (v >> 8); }
struct FEChunk {
    unsigned long id;   // +0x0
    unsigned long size; // +0x4
};
// A tagged value inside a chunk.
struct FETag {
    unsigned short id;   // +0x0
    unsigned short size; // +0x2
};
static inline unsigned long FETagLong(FETag *tag, int index) { return FESwap(((unsigned long *)(tag + 1))[index]); }
struct FETypeSize {
    unsigned long type; // +0x0
    unsigned long size; // +0x4
};
// The header chunk's body; the package name and file name follow it.
struct FEPackageHeader {
    unsigned long version;     // +0x0
    unsigned long unknown_4;   // +0x4
    unsigned long objectCount; // +0x8
    unsigned long scriptCount; // +0xc
    unsigned long nameLength;  // +0x10
    unsigned long unknown_14;  // +0x14
};
struct FEFileLibraryRef {
    unsigned long id;        // +0x0
    unsigned long unknown_4; // +0x4
    unsigned long unknown_8; // +0x8
};
class FEPackageReader {
  public:
    FEPackageReader();
    ~FEPackageReader();
    void Reset();
    FEPackage *Load(const void *, FEGameInterface *, FEngine *, bool, bool, bool);
    FEChunk *FindChild(FEChunk *, unsigned long);
    unsigned long GetTypeSize(unsigned long);
    int ReadTypeSizes();
    int ReadHeaderChunk();
    int ReadReferencedPackagesChunk();
    int ReadLibraryRefsChunk();
    int ReadResourceChunk();
    int ReadPackageResponseChunk();
    int ReadObjectChunk();
    FEObject *CreateObject(unsigned long);
    int ReadObjectTags(FETag *, unsigned long);
    int ReadScriptTags(FETag *, unsigned long);
    int ReadMessageResponseTags(FETag *, unsigned long, bool);
    int ReadMessageTargetListChunk();
    void ProcessStringTag(FETag *);
    void ProcessImageTag(FETag *);
    void ProcessListBoxTag(FETag *);
    void ProcessCodeListBoxTag(FETag *);
    int FindReferencedObject(unsigned long, FEObject **, FEPackage **);
    FEPackage *package;       // +0x00
    FEChunk *root;            // +0x04
    int skipInterp;           // +0x08; ReadObjectChunk skips FEKeyInterp when set.
    int isLibrary;            // +0x0c
    int keepExisting;         // +0x10
    FEObject *object;         // +0x14; the object being read.
    FEObject *parent;         // +0x18; the last object an 'AP' tag named.
    FEGroup *parentGroup;     // +0x1c
    int isReference;          // +0x20; the object was cloned from a library package.
    FEObject *referencedObject;   // +0x24
    FEPackage *referencedPackage; // +0x28
    long columnIndex;         // +0x2c; the list box tag being read.
    long rowIndex;            // +0x30
    long cellIndex;           // +0x34
    unsigned long typeCount;  // +0x38
    FETypeSize *types;        // +0x3c
    FEGameInterface *game;    // +0x40
    FEngine *engine;          // +0x44
    unsigned long objectCount; // +0x48
    unsigned long scriptCount; // +0x4c
    unsigned long buttonCount; // +0x50
    unsigned long buttonIndex; // +0x54; next free slot in the package's button table.
};

#endif
