// AI-assisted GR8E69 reconstruction; see docs/research/feng-packages.md.
#ifdef DECOMP_COMPARE
#include "FETypeNode.h"
#include "FEngStrings.h"
#include <string.h>
void FEFieldNode::SetDefault(void *source) {
    if (value) delete[] value;
    value = 0;
    if (size) {
        value = new ("source/FEngine/FETypeNode.cpp:21") unsigned char[size];
        memcpy(value, source, size);
    }
}
void FETypeNode::AddField(const char *name, long kind) {
    FEFieldNode *field = new ("source/FEngine/FETypeNode.cpp:36") FEFieldNode;
    field->SetName(name);
    field->keyType = kind;
    field->size = FEKeyTypeSize[kind];
    fields.AddNode(fields.tail, field);
    UpdateOffsets();
}
void FETypeNode::UpdateOffsets() {
    long offset = 0;
    for (FEFieldNode *field = (FEFieldNode *)fields.head; field; field = (FEFieldNode *)field->next) {
        field->offset = offset;
        offset += field->size;
    }
}
FEFieldNode *FETypeNode::GetField(const char *name) {
    FEFieldNode *field = (FEFieldNode *)fields.head;
    while (field && strcasecmp(field->name, name)) field = (FEFieldNode *)field->next;
    return field;
}
#endif
