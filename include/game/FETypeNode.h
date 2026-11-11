// FEng field/type nodes; symbol-backed class names and observed member offsets.
#ifndef GAME_FE_TYPE_NODE_H
#define GAME_FE_TYPE_NODE_H
#pragma interface
#include "FEngLists.h"
#include "FEngStrings.h"
class FEFieldNode : public FENode {
  public:
    long keyType;          // +0x14
    long size;             // +0x18
    long offset;           // +0x1c
    unsigned char *value;  // +0x20
    FEFieldNode() : size(0), offset(0), value(0) {}
    void *operator new(unsigned int size, const char *label) { return DWI_alloc(label, size, 256); }
    virtual ~FEFieldNode();
    void SetDefault(void *);
};
class FETypeNode : public FENode {
  public:
    FEMinList fields;      // +0x14
    unsigned long type;    // +0x24
    FETypeNode() {}
    void *operator new(unsigned int size, const char *label) { return DWI_alloc(label, size, 256); }
    virtual ~FETypeNode();
    void AddField(const char *, long);
    void UpdateOffsets();
    FEFieldNode *GetField(const char *);
};
extern unsigned long FEKeyTypeSize[];
#endif
