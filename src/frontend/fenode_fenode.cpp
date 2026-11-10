// FEPackage and FENode: construction and teardown.
#include "FEngLists.h"
#include "FEngine.h"
#include "FEngStrings.h"
#include "FEHash.h"
#include <string.h>
// Nodes in FEPackage::pendingList; the type is inferred from the destructor's walk.
class FEPackageTextNode : public FEMinNode {
  public:
    unsigned long unknown_c; // +0xc
    char *text;              // +0x10
    virtual ~FEPackageTextNode();
};

FENode::FENode() : name(0), hash(0) {}
FENode::~FENode() {
    if (name) delete[] name;
}
int FENode::SetName(const char *text) {
    int copied = 0;
    if (name) {
        delete[] name;
        name = 0;
    }
    if (text) {
        name = new ("source/FEngine/FEList.cpp:130") char[strlen(text) + 1];
        if (name) {
            strcpy(name, text);
            copied = 1;
        }
    }
    hash = FEHashUpper(name);
    return copied;
}
