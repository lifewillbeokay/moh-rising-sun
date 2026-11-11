// AI-assisted GR8E69 reconstruction; see docs/research/feng-packages.md.
#include "FEngine.h"
#include "FETypeNode.h"
FETypeNode *FETypeLib::FindType(unsigned long kind) {
    FETypeNode *node = (FETypeNode *)types.head;
    while (node && node->type != kind) node = (FETypeNode *)node->next;
    return node;
}
