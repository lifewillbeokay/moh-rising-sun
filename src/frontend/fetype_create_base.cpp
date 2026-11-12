// AI-assisted GR8E69 reconstruction; see docs/research/feng-lifecycle.md.
#ifdef DECOMP_COMPARE
#include "FEngine.h"
#include "FETypeNode.h"
#include "FEngInterpolation.h"
FETypeNode *FETypeLib::CreateBaseObjectType(const char *name) {
    FEVector3 zero(0, 0, 0), one(0, 0, 0);
    FEQuaternion rotation = {0, 0, 0, 1};
    FEColor color;
    FETypeNode *type = new ("source/FEngine/FETypeLib.cpp:39") FETypeNode;
    type->SetName(name);
    type->AddField("Color", 6); type->AddField("Pivot", 4);
    type->AddField("Position", 4); type->AddField("Rotation", 5); type->AddField("Size", 4);
    zero.x = zero.y = zero.z = 0;
    one.x = one.y = one.z = 1;
    rotation.x = rotation.y = rotation.z = 0; rotation.w = 1;
    color = FEColor(0xffffffff);
    FEFieldNode *field = (FEFieldNode *)type->fields.head;
    field->SetDefault(&color); field = (FEFieldNode *)field->next;
    field->SetDefault(&zero); field = (FEFieldNode *)field->next;
    field->SetDefault(&zero); field = (FEFieldNode *)field->next;
    field->SetDefault(&rotation); field = (FEFieldNode *)field->next;
    field->SetDefault(&one);
    return type;
}
#endif
